#include "gameloop.h"
#include "ranking_calculator.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

#include <netinet/in.h>

#include "../../common/Dto/checkpoint.h"
#include "../../common/Dto/close_game.h"
#include "../../common/Dto/end_race.h"
#include "../../common/Dto/game_finished.h"
#include "../../common/Dto/infinite_health.h"
#include "../../common/Dto/initial_race_map.h"
#include "../../common/Dto/lap_completed.h"
#include "../../common/Dto/npc.h"
#include "../../common/Dto/player.h"
#include "../../common/Dto/player_game_finished.h"
#include "../../common/Dto/player_hit_npc.h"
#include "../../common/Dto/player_move.h"
#include "../../common/Dto/player_race_finished.h"
#include "../../common/Dto/race_finished.h"
#include "../../common/Dto/vehicle.h"
#include "../../common/Dto/vehicle_checkpoint.h"
#include "../../common/Dto/vehicle_collision.h"
#include "../../common/Dto/vehicle_exploded.h"
#include "../../common/Dto/vehicle_upgrade.h"
#include "../../common/Dto/vehicle_wall_collision.h"
#include "../../common/common_codes.h"
#include "../../common/vehicle_type_utils.h"
#include "../YamlParser.h"
#include "../constants.h"
#include "../physics/LevelCreator.h"

#define GAME_TICK_MS 16

using Clock = std::chrono::steady_clock;
using Milliseconds = std::chrono::milliseconds;
using Seconds = std::chrono::seconds;

GameLoop::GameLoop(Queue<std::shared_ptr<Dto>>& gameLoopQueue, std::map<int, CarConfig>& chosenCars,
                   std::map<int, std::string>& playerUsernames, Broadcaster& broadcaster,
                   int maxPlayers):
        gameLoopQueue(gameLoopQueue),
        chosenCars_(chosenCars),
        playerUsernames_(playerUsernames),
        broadcaster_(broadcaster),
        maxPlayers(maxPlayers),
        raceActive_(false),
        pendingNextRace_(false) {}

void GameLoop::addSelectedMapPath(const std::string& path) { selectedMaps_.push_back(path); }

void GameLoop::run() {
    try {
        buildRacesFromSelectedMaps();
        if (!races_.empty()) {
            startRace(0);  // primera carrera
        }
        bool closingGame = false;
        Clock::time_point closeGameTime;

        while (should_keep_running()) {
            if (closingGame) {
                if (Clock::now() >= closeGameTime) {
                    std::cout << "[GameLoop] Closing game loop and notifying clients.\n";
                    broadcaster_.broadcast(std::make_shared<CloseGameDto>());
                }
            }
            processCommands();

            if (raceActive_ && setup.has_value()) {
                updateCountdown();
                sendVehiclesPositions();
                processGameEvents();
            }

            if (!raceActive_ && pendingNextRace_) {
                if (Clock::now() >= nextRaceStartTime_) {
                    int nextRaceIndex = currentRaceIndex_ + 1;

                    if (nextRaceIndex < (int)races_.size()) {
                        startRace(nextRaceIndex);

                    } else {
                        pendingNextRace_ = false;
                        sendFinalResults();
                        closingGame = true;
                        closeGameTime = Clock::now() + std::chrono::seconds(5);
                    }
                }
            }


            std::this_thread::sleep_for(std::chrono::milliseconds(GAME_TICK_MS));
        }
    } catch (const ClosedQueue& e) {
        return;
    }
}

bool GameLoop::isLastRace() const { return currentRaceIndex_ == (int)races_.size() - 1; }


void GameLoop::sendFinalResults() {
    auto rankings = RankingCalculator::calculateRankings(playerResults_, playerUsernames_);

    int position = 1;
    for (const auto& e : rankings) {
        auto finalResultDto = std::make_shared<PlayerGameFinishedDto>(
                e.username, e.totalTime, e.penalties, position);
        broadcaster_.broadcast(finalResultDto);
        position++;
    }
    broadcaster_.broadcast(std::make_shared<GameFinishedDto>());
}

void GameLoop::buildRacesFromSelectedMaps() {
    if (!races_.empty())
        return;

    YamlParser parser;

    for (const auto& filename: selectedMaps_) {
        RaceInfo info = parser.parseRaceInfo("../server/maps/" + filename);
        races_.push_back(std::move(info));
    }

    if (races_.empty()) {
        std::cout << "[GameLoop] WARNING: races_ está vacío, no hay carreras configuradas\n";
    }
}

void GameLoop::startRace(int raceIndex) {
    if (raceIndex < 0 || raceIndex >= (int)races_.size()) {
        std::cerr << "[GameLoop] invalid raceIndex " << raceIndex << "\n";
        return;
    }

    currentRaceIndex_ = raceIndex;
    pendingNextRace_ = false;

    const std::string vehiclesYaml = "../server/vehicles_specs/vehicle_specs.yaml";
    const RaceInfo& race = races_[raceIndex];

    // conseguimos el path de físicas a partir del nombre del mapa
    std::string direccion = levelDirForMap(race.mapName);

    // usamos los checkpoints y spawns que vinieron del archivo
    setup.emplace(direccion, vehiclesYaml, chosenCars_, upgradesByUser_, race.checkpoints,
                  race.spawns);

    raceProgress_.clear();
    for (const auto& [playerId, _]: chosenCars_) {
        raceProgress_[playerId] = {};
    }

    // nombre visible de mapa para el cliente
    sendMapName(race.mapName);
    sendCheckpoints();
    sendInitialPlayersCars();
    sendNpcPositions();

    setVehiclesControlEnabled(false);
    countdownActive_ = true;
    countdownStartTime_ = Clock::now();

    raceActive_ = true;
    raceStartTime_ = Clock::now();
}

std::string GameLoop::levelDirForMap(const std::string& mapName) {
    std::string id = mapName;
    if (id.size() > 4 && id.substr(id.size() - 4) == ".png") {
        id = id.substr(0, id.size() - 4);
    }
    if (id == "liberty_city") {
        return "../server/physics/Levels/Liberty_City";
    }
    if (id == "san_andreas") {
        return "../server/physics/Levels/San_Andreas";
    }
    if (id == "vice_city") {
        return "../server/physics/Levels/Vice_City";
    }

    std::cerr << "[GameLoop] levelDirForMap: unknown mapName " << mapName << "\n";
    // fallback para no crashear:
    return "../server/physics/Levels/Liberty_City";
}


void GameLoop::updateCountdown() {
    if (!countdownActive_)
        return;

    auto now = Clock::now();
    auto elapsedMs =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - countdownStartTime_)
                    .count();

    if (elapsedMs >= 7000) {  // 3 segundos
        setVehiclesControlEnabled(true);
        countdownActive_ = false;
    }
}


void GameLoop::setVehiclesControlEnabled(bool enabled) {
    if (!setup.has_value())
        return;

    for (auto& [player_id, vehicle]: setup->getVehicleMap()) {
        if (!vehicle)
            continue;

        if (enabled) {
            vehicle->enableControl();
        } else {
            vehicle->disableControl();
        }
    }
}

void GameLoop::sendNpcPositions() {
    if (!setup.has_value())
        return;
    auto npcs = setup->getNpcs();

    for (const auto& npc: npcs) {
        std::cout << "mandando npc positionn \n";
        auto dto = std::make_shared<NPCDto>(npc.id, npc.x_px, npc.y_px);
        broadcaster_.broadcast(dto);
    }
}

void GameLoop::sendMapName(std::string mapName) {
    auto dto = std::make_shared<InitialRaceMapDto>(mapName);
    broadcaster_.broadcast(dto);
}

void GameLoop::sendCheckpoints() {
    for (const auto& [x_px, y_px, index]: setup->getCheckpoints()) {
        auto dto = std::make_shared<CheckpointDto>(index, x_px, y_px);
        std::cout << "Enviando dto: " << dto->id << ", " << dto->x << ", " << dto->y << std::endl;
        broadcaster_.broadcast(dto);
    }
}

void GameLoop::sendInitialPlayersCars() {
    for (const auto& [playerId, cfg]: chosenCars_) {
        VehicleTipe vt;
        try {
            vt = toVehicleTipe(cfg.carType);
        } catch (const std::exception& e) {
            std::cerr << "[sendInitialPlayersCars] player " << playerId << " has invalid carType"
                      << cfg.carType << " (" << e.what() << ")\n";
            continue;
        }

        std::string username = playerUsernames_.at(playerId);
        auto vehicle = getVehicleById(playerId);
        auto dto = std::make_shared<PlayerDto>(username, vt, vehicle->getVehicleHp());
        broadcaster_.broadcast(dto);
    }
}

void GameLoop::processCommands() {
    std::shared_ptr<Dto> command;
    while (gameLoopQueue.try_pop(command)) {
        handlerProcessCommand(command);
    }
}

void GameLoop::handlerProcessCommand(std::shared_ptr<Dto> command) {
    switch (static_cast<ActionCode>(command->return_code())) {
        case ActionCode::SEND_PLAYER_MOVE: {
            auto player_move_dto = std::dynamic_pointer_cast<PlayerMoveDto>(command);
            if (!player_move_dto) {
                std::cerr << "[GameLoop] SEND_PLAYER_MOVE: bad dto cast\n";
                break;
            }

            Vehicle* vehicle = getVehicleByPlayer(command->get_username());
            if (!vehicle) {
                std::cerr << "[GameLoop] SEND_PLAYER_MOVE: no vehicle for user "
                          << command->get_username() << "\n";
                break;
            }

            uint8_t mask = player_move_dto->move;

            if (mask & static_cast<uint8_t>(MoveMask::ACCELERATE))
                vehicle->accelerate();

            if (mask & static_cast<uint8_t>(MoveMask::BRAKE))
                vehicle->brake();

            if (mask & static_cast<uint8_t>(MoveMask::TURN_LEFT))
                vehicle->turn(TurnDir::Left);

            if (mask & static_cast<uint8_t>(MoveMask::TURN_RIGHT))
                vehicle->turn(TurnDir::Right);
            break;
        }
        case ActionCode::SEND_INFINITE_HEALTH: {
            auto player_infinite_health = std::dynamic_pointer_cast<InfiniteHealthDto>(command);
            if (!player_infinite_health) {
                std::cerr << "[GameLoop] SEND_INFINITE_HEALTH: bad dto cast\n";
                break;
            }

            auto player_vehicle = getVehicleByPlayer(player_infinite_health->username);
            if (!player_vehicle) {
                std::cerr << "[GameLoop] SEND_INFINITE_HEALTH: no vehicle for user "
                          << player_infinite_health->username << "\n";
                break;
            }

            player_vehicle->setInfiniteHp();
            break;
        }
        case ActionCode::SEND_END_RACE: {
            auto player_end_race = std::dynamic_pointer_cast<EndRaceDto>(command);
            if (!player_end_race) {
                std::cerr << "[GameLoop] SEND_END_RACE: bad dto cast\n";
                break;
            }

            auto player_vehicle = getVehicleByPlayer(player_end_race->username);
            if (!player_vehicle) {
                std::cerr << "[GameLoop] SEND_END_RACE: no vehicle for user "
                          << player_end_race->username << "\n";
                break;
            }

            int vehicleId = player_vehicle->getVehicleId();
            auto it = raceProgress_.find(vehicleId);
            if (it != raceProgress_.end()) {
                onPlayerFinished(vehicleId, it->second);
            } else {
                std::cerr << "[GameLoop] SEND_END_RACE: no raceProgress for vehicleId " << vehicleId
                          << "\n";
            }
            break;
        }
        case ActionCode::SEND_VEHICLE_UPGRADE: {
            auto upgradeDto = std::dynamic_pointer_cast<VehicleUpgradeDto>(command);
            if (!upgradeDto) {
                std::cerr << "[GameLoop] SEND_VEHICLE_UPGRADE: bad dto cast\n";
                break;
            }

            Vehicle* vehicle = getVehicleByPlayer(upgradeDto->username);
            if (!vehicle) {
                std::cerr << "[GameLoop] SEND_VEHICLE_UPGRADE: no vehicle for user "
                          << upgradeDto->username << "\n";
                break;
            }

            int vehicleId = vehicle->getVehicleId();

            auto& up = upgradesByUser_[vehicleId];

            if (upgradeDto->healthUpgrade) {
                up.armorLevel += 1;
                playerResults_[vehicleId].totalPenaltySeconds += 3.0f;
                std::cout << "[GameLoop] " << upgradeDto->username
                          << " upgraded ARMOR. New level=" << up.armorLevel << "\n";
            }

            if (upgradeDto->speedUpgrade) {
                up.engineLevel += 1;
                playerResults_[vehicleId].totalPenaltySeconds += 3.0f;
                std::cout << "[GameLoop] " << upgradeDto->username
                          << " upgraded ENGINE. New level=" << up.engineLevel << "\n";
            }

            break;
        }
        default:
            std::cerr << "[GameLoop] unknown command: " << command->return_code() << "\n";
            break;
    }
}


void GameLoop::sendVehiclesPositions() {

    for (auto& [player_id, vehicle]: setup.value().getVehicleMap()) {
        float x, y, angle;
        vehicle->getPosition(x, y, angle);
        std::string username = playerUsernames_.at(player_id);
        auto dto = std::make_shared<VehicleDto>(username, x, y, angle,
                                                setup->getVehicleSpeed(player_id),
                                                vehicle->getVehicleHp(), vehicle->getUnderBridge());
        broadcaster_.broadcast(dto);
    }
}

Vehicle* GameLoop::getVehicleByPlayer(const std::string& username) {
    int foundId = -1;

    for (const auto& [id, name]: playerUsernames_) {
        if (name == username) {
            foundId = id;
            break;
        }
    }

    if (foundId == -1)
        return nullptr;

    const auto& map = setup->getVehicleMap();
    auto it = map.find(foundId);
    return (it == map.end()) ? nullptr : it->second.get();
}

Vehicle* GameLoop::getVehicleById(int vehicleId) {
    auto it = playerUsernames_.find(vehicleId);
    if (it == playerUsernames_.end()) {
        return nullptr;
    }

    const std::string& username = it->second;
    return getVehicleByPlayer(username);
}

bool GameLoop::allPlayersFinished() {
    std::cerr << "\n[allPlayersFinished] chequeando...\n";
    std::cerr << "  chosenCars_.size() = " << chosenCars_.size() << "\n";

    // Logueamos el estado de cada jugador
    for (const auto& [playerId, _]: chosenCars_) {
        auto it = raceProgress_.find(playerId);
        if (it == raceProgress_.end()) {
            std::cerr << "  playerId " << playerId << ": NO ENTRY in raceProgress_\n";
        } else {
            std::cerr << "  playerId " << playerId << ": finished=" << it->second.finished << "\n";
        }
    }

    // Ahora chequeamos de verdad y logueamos el resultado
    for (const auto& [playerId, _]: chosenCars_) {
        auto it = raceProgress_.find(playerId);
        if (it == raceProgress_.end() || !it->second.finished) {
            std::cerr << "[allPlayersFinished] -> false "
                      << "(playerId " << playerId << " aún no terminó)\n";
            return false;
        }
    }

    std::cerr << "[allPlayersFinished] -> true (todos terminaron)\n";
    return true;
}

int GameLoop::computePlayerPosition(int vehicleId) {
    int finishedBefore = 0;

    for (const auto& [id, st]: raceProgress_) {
        if (id == vehicleId)
            continue;
        if (st.finished) {
            finishedBefore++;
        }
    }
    return finishedBefore + 1;
}

void GameLoop::onPlayerFinished(int vehicleId, PlayerRaceProgress& prog) {
    if (prog.finished) {
        return;
    }

    prog.finished = true;
    prog.finishTime = Clock::now();

    auto elapsed = *prog.finishTime - raceStartTime_;
    auto ms = std::chrono::duration_cast<Milliseconds>(elapsed).count();
    float seconds = static_cast<float>(ms) / 1000.0f;

    int position = computePlayerPosition(vehicleId);
    playerResults_[vehicleId].totalTimeSeconds += seconds;

    auto finishDto = std::make_shared<PlayerRaceFinishedDto>(playerUsernames_.at(vehicleId),
                                                             seconds, position);
    broadcaster_.broadcast(finishDto);

    if (auto* vehicle = getVehicleById(vehicleId)) {
        vehicle->disableControl();
    }

    if (allPlayersFinished()) {
        std::cout
                << "[onPlayerFinished] -> allPlayersFinished() == true, ENVIANDO RaceFinishedDto\n";
        raceActive_ = false;
        pendingNextRace_ = true;
        nextRaceStartTime_ = Clock::now() + std::chrono::seconds(10);
        if (!isLastRace()) {
            auto raceFinish = std::make_shared<RaceFinishedDto>();
            broadcaster_.broadcast(raceFinish);
        }
    }
}

void GameLoop::handleRaceProgress(int vehicleId, int checkpointIndex) {
    auto& prog = raceProgress_[vehicleId];

    if (prog.finished) {
        return;
    }

    if (checkpointIndex != prog.nextCheckpoint) {
        return;
    }

    auto cpDto =
            std::make_shared<VehicleCheckpointDto>(playerUsernames_.at(vehicleId), checkpointIndex);
    broadcaster_.broadcast(cpDto);

    if (checkpointIndex == setup->totalCheckpoints() - 1) {
        prog.currentLap++;
        prog.nextCheckpoint = 0;

        auto lapDto =
                std::make_shared<LapCompletedDto>(playerUsernames_.at(vehicleId), prog.currentLap);
        broadcaster_.broadcast(lapDto);

        if (prog.currentLap >= setup->totalLaps()) {
            onPlayerFinished(vehicleId, prog);
        }

    } else {
        prog.nextCheckpoint++;
    }
}

void GameLoop::handleVehicleExplosion(int vehicleId) {
    Vehicle* vehicle = getVehicleById(vehicleId);
    if (!vehicle)
        return;

    vehicle->disableControl();

    auto explDto = std::make_shared<VehicleExplodedDto>(playerUsernames_.at(vehicleId));
    broadcaster_.broadcast(explDto);

    auto it = raceProgress_.find(vehicleId);
    if (it != raceProgress_.end()) {
        it->second.finished = true;
    }

    if (allPlayersFinished()) {
        std::cout << "[handleVehicleExplosion] -> allPlayersFinished() == true, ENVIANDO "
                     "RaceFinishedDto\n";
        raceActive_ = false;
        pendingNextRace_ = true;
        nextRaceStartTime_ = Clock::now() + std::chrono::seconds(10);
        auto raceFinish = std::make_shared<RaceFinishedDto>();
        broadcaster_.broadcast(raceFinish);
    }
}

float GameLoop::computeCollisionDamage(float impactSpeed) {
    const float minSpeed = 5.0f;
    const float maxSpeed = 25.0f;
    const float maxDamage = 40.0f;

    if (impactSpeed <= minSpeed)
        return 0.0f;
    if (impactSpeed >= maxSpeed)
        return maxDamage;

    float t = (impactSpeed - minSpeed) / (maxSpeed - minSpeed);
    return t * maxDamage;
}

void GameLoop::handleVehicleVehicleCollision(const RawVehicleVehicle& event) {
    Vehicle* vehicleA = getVehicleById(event.a);
    Vehicle* vehicleB = getVehicleById(event.b);

    if (!vehicleA && !vehicleB)
        return;


    float dmgA = computeCollisionDamage(event.speedA);
    if (dmgA > 0.0f) {
        vehicleA->applyDamage(dmgA);
    }

    float dmgB = computeCollisionDamage(event.speedB);
    if (dmgB > 0.0f) {
        vehicleB->applyDamage(dmgB);
    }

    auto dto = std::make_shared<VehicleCollisionDto>(
            playerUsernames_.at(event.a), vehicleA->getVehicleHp(), playerUsernames_.at(event.b),
            vehicleB->getVehicleHp());
    broadcaster_.broadcast(dto);

    if (vehicleA->getVehicleHp() == 0) {
        handleVehicleExplosion(event.a);
    }
    if (vehicleB->getVehicleHp() == 0) {
        handleVehicleExplosion(event.b);
    }
}

void GameLoop::handleVehicleWallCollision(const RawVehicleWall& event) {
    Vehicle* vehicle = getVehicleById(event.vehicleId);
    if (!vehicle)
        return;

    float dmg = computeCollisionDamage(event.speedBeforeImpact);
    if (dmg > 0.0f) {
        vehicle->applyDamage(dmg);
    }

    auto dto = std::make_shared<VehicleWallCollisionDto>(playerUsernames_.at(event.vehicleId),
                                                         vehicle->getVehicleHp());
    broadcaster_.broadcast(dto);

    if (vehicle->getVehicleHp() == 0) {
        handleVehicleExplosion(event.vehicleId);
    }
}

void GameLoop::handleVehicleBridgeToggle(const RawVehicleBridgeToggle& event) {
    Vehicle* vehicle = getVehicleById(event.vehicleId);
    if (!vehicle)
        return;
    vehicle->setUnderBridge(event.under);
}

void GameLoop::handleVehicleNpcCollision(const RawVehicleNpc& event) {
    if (!setup.has_value())
        return;

    Vehicle* vehicle = getVehicleById(event.vehicleId);
    if (!vehicle)
        return;

    LevelSetup& level = setup.value();
    if (!level.isNpcAlive(event.npcId)) {
        return;
    }

    std::cout << "PISANTE NPC\n";
    level.markNpcDead(event.npcId);
    vehicle->CarHitNpc();

    auto it = playerUsernames_.find(event.vehicleId);
    if (it == playerUsernames_.end())
        return;

    auto dto = std::make_shared<PlayerHitNPCDto>(it->second, event.npcId);
    broadcaster_.broadcast(dto);
}

void GameLoop::processGameEvents() {
    auto events = setup->stepAndDrainEvents(1.0f / 60.0f);

    for (auto& event: events) {
        if (auto* vehicle_checkpoint = std::get_if<RawVehicleCheckpoint>(&event)) {

            handleRaceProgress(vehicle_checkpoint->vehicleId, vehicle_checkpoint->checkpointIndex);

        } else if (auto* vehicle_vehicle = std::get_if<RawVehicleVehicle>(&event)) {

            handleVehicleVehicleCollision(*vehicle_vehicle);

        } else if (auto* vehicle_wall = std::get_if<RawVehicleWall>(&event)) {

            handleVehicleWallCollision(*vehicle_wall);
        } else if (auto* vehicle_BridgeToggle = std::get_if<RawVehicleBridgeToggle>(&event)) {
            handleVehicleBridgeToggle(*vehicle_BridgeToggle);

        } else if (auto* vehicle_npc = std::get_if<RawVehicleNpc>(&event)) {
            handleVehicleNpcCollision(*vehicle_npc);
        }
    }
}

GameLoop::~GameLoop() {
    try {
        if (should_keep_running()) {
            stop();
        }
        if (is_alive()) {
            join();
        }
    } catch (...) {}
}
