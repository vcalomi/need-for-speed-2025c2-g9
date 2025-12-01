#include "gameloop.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

#include <netinet/in.h>

#include "../../common/Dto/checkpoint.h"
#include "../../common/Dto/lap_completed.h"
#include "../../common/Dto/player.h"
#include "../../common/Dto/player_move.h"
#include "../../common/Dto/player_race_finished.h"
#include "../../common/Dto/race_finished.h"
#include "../../common/Dto/vehicle.h"
#include "../../common/Dto/vehicle_checkpoint.h"
#include "../../common/Dto/vehicle_collision.h"
#include "../../common/Dto/vehicle_exploded.h"
#include "../../common/Dto/vehicle_wall_collision.h"
#include "../../common/Dto/initial_race_map.h"
#include "../../common/Dto/race_finished.h"
#include "../../common/Dto/end_race.h"
#include "../../common/Dto/infinite_health.h"
#include "../../common/Dto/vehicle_upgrade.h"
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
                   int maxPlayers, const std::vector<std::string>& selectedMaps):
        gameLoopQueue(gameLoopQueue),
        chosenCars_(chosenCars),
        playerUsernames_(playerUsernames),
        broadcaster_(broadcaster),
        maxPlayers(maxPlayers),
        raceActive_(false),
        pendingNextRace_(false)
{
    levels_.push_back(LevelInfo{"../server/physics/Levels/Liberty_City", "liberty_city"});
    levels_.push_back(LevelInfo{"../server/physics/Levels/San_Andreas", "liberty_city"});
    
    // POngo esto para evitar el warning
    (void)selectedMaps;
}

void GameLoop::run() {
    try {
        startRace(0);

        while (should_keep_running()) {
            processCommands();

            if (raceActive_ && setup.has_value()) {
                sendVehiclesPositions();
                processGameEvents();
            }

            if (!raceActive_ && pendingNextRace_) {
                if (Clock::now() >= nextRaceStartTime_) {
                    if (currentLevelIndex_ == 0 && levels_.size() > 1) {
                        startRace(1);
                    }
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(GAME_TICK_MS));
        }
    } catch (const ClosedQueue& e) {
        return;
    }
}

void GameLoop::startRace(int levelIndex) {
    if (levelIndex < 0 || levelIndex >= (int)levels_.size()) {
        std::cerr << "[GameLoop] invalid levelIndex " << levelIndex << "\n";
        return;
    }

    currentLevelIndex_ = levelIndex;
    pendingNextRace_ = false;

    const std::string vehiclesYaml = "../server/vehicles_specs/vehicle_specs.yaml";
    LevelInfo level = levels_[levelIndex];

    setup.emplace(level.dir, vehiclesYaml, chosenCars_, upgradesByUser_);

    raceProgress_.clear();
    for (const auto& [playerId, _]: chosenCars_) {
        raceProgress_[playerId] = {};
    }

    sendMapName(level.mapName);
    sendCheckpoints();
    sendInitialPlayersCars();


    raceActive_ = true;
    raceStartTime_ = Clock::now();
}

void GameLoop::sendMapName(std::string mapName){
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
            auto player_infinite_health =
                std::dynamic_pointer_cast<InfiniteHealthDto>(command);
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
                std::cerr << "[GameLoop] SEND_END_RACE: no raceProgress for vehicleId "
                          << vehicleId << "\n";
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
                std::cout << "[GameLoop] " << upgradeDto->username
                          << " upgraded ARMOR. New level=" << up.armorLevel << "\n";
            }

            if (upgradeDto->speedUpgrade) {
                up.engineLevel += 1;
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
        auto dto = std::make_shared<VehicleDto>(username, x, y, angle, setup->getVehicleSpeed(player_id), vehicle->getUnderBridge());
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
    for (const auto& [playerId, _] : chosenCars_) {
        auto it = raceProgress_.find(playerId);
        if (it == raceProgress_.end()) {
            std::cerr << "  playerId " << playerId 
                      << ": NO ENTRY in raceProgress_\n";
        } else {
            std::cerr << "  playerId " << playerId 
                      << ": finished=" << it->second.finished << "\n";
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

int GameLoop::computePlayerPosition(int vehicleId){
    int finishedBefore = 0;

    for (const auto& [id, st] : raceProgress_) {
        if (id == vehicleId) continue;
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

    auto finishDto = std::make_shared<PlayerRaceFinishedDto>(
        playerUsernames_.at(vehicleId),
        seconds,
        position
    );
    broadcaster_.broadcast(finishDto);

    if (auto* vehicle = getVehicleById(vehicleId)) {
        vehicle->disableControl();
    }

    if (allPlayersFinished()) {
        std::cout << "[onPlayerFinished] -> allPlayersFinished() == true, ENVIANDO RaceFinishedDto\n";
        raceActive_ = false;
        pendingNextRace_ = true;
        nextRaceStartTime_ = Clock::now() + std::chrono::seconds(10);
        auto raceFinish = std::make_shared<RaceFinishedDto>();
        broadcaster_.broadcast(raceFinish);
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
        std::cout << "[handleVehicleExplosion] -> allPlayersFinished() == true, ENVIANDO RaceFinishedDto\n";
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

void GameLoop::handleVehicleBridgeToggle(const RawVehicleBridgeToggle& event){
    Vehicle* vehicle = getVehicleById(event.vehicleId);
    if (!vehicle)
        return;
    vehicle->setUnderBridge(event.under);
    std::cout << "[BridgeToggle] Vehicle " << event.vehicleId
              << " is now " << (event.under ? "UNDER" : "OVER")
              << " the bridge.\n";
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
        } else if (auto* vehicle_BridgeToggle = std::get_if<RawVehicleBridgeToggle>(&event) ){
            handleVehicleBridgeToggle(*vehicle_BridgeToggle);
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
