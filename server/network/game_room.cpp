#include "game_room.h"

#include <algorithm>
#include <iostream>

#include "../../common/Dto/vehicle.h"
#include "../../common/Dto/player_left.h"
#include "../../common/Dto/lobby_room_state.h"
#include "../../common/common_codes.h"
#include "../../common/queue.h"

GameRoom::GameRoom(const std::string& roomName, int hostId, int maxPlayers):
        roomName(roomName),
        hostId(hostId),
        maxPlayers_(maxPlayers),
        state(RoomState::WAITING_FOR_PLAYERS),
        gameQueue(),
        broadcaster(),
        gameLoop(gameQueue, chosenCars, playerUsernames, broadcaster, maxPlayers, selectedMaps) {}

bool GameRoom::addPlayer(int clientId, Player* player) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!canJoin()) return false;

    lobbyPlayers[clientId] = player;
    broadcaster.addQueue(&player->getSendQueue());
    return true;
}

void GameRoom::addGamePlayer(std::shared_ptr<PlayerInfo> player_info) {
    std::lock_guard<std::mutex> lock(mtx);
    int clientId = player_info->getPlayerId();

    auto lobbyIt = lobbyPlayers.find(clientId);
    if (lobbyIt != lobbyPlayers.end()) {
        broadcaster.removeQueue(&lobbyIt->second->getSendQueue());
        lobbyPlayers.erase(lobbyIt);
    }

    gamePlayers[clientId] = std::make_unique<Player>(
        player_info->getProtocol(),
        clientId
    );

    playerUsernames[clientId] = player_info->getUsername();
    chosenCars[clientId] = CarConfig{player_info->getCarType()};

    auto* queue = &gamePlayers[clientId]->getSendQueue();
    if (queue != nullptr) {
        broadcaster.addQueue(queue);
    }
}

bool GameRoom::setPlayerUsername(int clientId, const std::string& username) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& [id, existingUsername]: playerUsernames) {
        if (existingUsername == username && id != clientId) {
            return false;
        }
    }

    playerUsernames[clientId] = username;
    return true;
}

void GameRoom::setSelectedMaps(const std::vector<uint8_t>& mapIds) {
    std::lock_guard<std::mutex> lock(mtx);
    selectedMaps.clear();
    for (uint8_t mapCode : mapIds) {
        selectedMaps.push_back(mapCodeToString(static_cast<MapCode>(mapCode)));
    }
}

std::string GameRoom::getCarType(int clientId) const {
    auto it = chosenCars.find(clientId);
    if (it != chosenCars.end()) {
        return it->second.carType;
    }
    return "jeep";
}

bool GameRoom::allPlayersHaveCars() const {
    for (const auto& [clientId, player] : lobbyPlayers) {
        if (chosenCars.find(clientId) == chosenCars.end()) {
            return false;
        }
    }
    return true;
}

bool GameRoom::startGame() {
    std::lock_guard<std::mutex> lock(mtx);
    if (state != RoomState::WAITING_FOR_PLAYERS) return false;
    if (lobbyPlayers.size() < 2) {
        return false;
    }

    for (const auto& [id, player] : lobbyPlayers) {
        if (chosenCars.find(id) == chosenCars.end()) {
            CarConfig defaultCar{};
            defaultCar.carType = "jeep";
            chosenCars[id] = defaultCar;
        }
    }

    state = RoomState::IN_RACE;
    return true;
}

void GameRoom::startGameForPlayer(int clientId) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = gamePlayers.find(clientId);
    if (it != gamePlayers.end() && it->second && !it->second->isInGame()) {
        it->second->startGame(gameQueue);
        checkAndStartGameLoop();
    }
}

void GameRoom::checkAndStartGameLoop() {
    size_t expectedPlayers = chosenCars.size();

    if (gamePlayers.size() == expectedPlayers) {
        gameLoop.start();
    }
}

void GameRoom::startGameForAllPlayers() {
    std::lock_guard<std::mutex> lock(mtx);
    
    for (auto& [id, player] : gamePlayers) {
        if (player && !player->isInGame()) {
            player->startGame(gameQueue);
        }
    }
    
    checkAndStartGameLoop();
}

void GameRoom::stopAllPlayers() {
    std::lock_guard<std::mutex> lock(mtx);

    try {
        gameQueue.close();
    } catch (...) {}

    for (auto& [id, player] : gamePlayers) {
        if (player) {
            try {
                player->stopGame();
            } catch (...) {}
        }
    }
    gamePlayers.clear();

    try {
        gameLoop.stop();
        if (gameLoop.is_alive()) {
            gameLoop.join();
        }
    } catch (...) {}
}

bool GameRoom::chooseCar(int clientId, const CarConfig& car) {
    std::lock_guard<std::mutex> lock(mtx);
    if (lobbyPlayers.find(clientId) == lobbyPlayers.end()) {
        return false;
    }

    chosenCars[clientId] = car;
    return true;
}

void GameRoom::removePlayer(int clientId) {
    std::lock_guard<std::mutex> lock(mtx);
    std::string leftUsername;
    auto uIt = playerUsernames.find(clientId);
    if (uIt != playerUsernames.end()) {
        leftUsername = uIt->second;
    }

    auto lobbyIt = lobbyPlayers.find(clientId);
    if (lobbyIt != lobbyPlayers.end()) {
        broadcaster.removeQueue(&lobbyIt->second->getSendQueue());
        lobbyPlayers.erase(lobbyIt);
    }

    auto gameIt = gamePlayers.find(clientId);
    if (gameIt != gamePlayers.end()) {
        broadcaster.removeQueue(&gameIt->second->getSendQueue());
        gamePlayers.erase(gameIt);
    }
    
    playerUsernames.erase(clientId);
    chosenCars.erase(clientId);
    if (!leftUsername.empty()) {
        auto dto = std::make_shared<PlayerLeftDto>(leftUsername);
        broadcaster.broadcast(dto);
    }
}

bool GameRoom::canJoin() const {
    return state == RoomState::WAITING_FOR_PLAYERS &&
           static_cast<int>(lobbyPlayers.size()) < maxPlayers_;
}

bool GameRoom::isHost(int clientId) const { return hostId == clientId; }

Queue<std::shared_ptr<Dto>>& GameRoom::getGameQueue() { return gameQueue; }

std::vector<int> GameRoom::getPlayerIds() {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<int> ids;
    ids.reserve(lobbyPlayers.size());
    for (const auto& player: lobbyPlayers) {
        ids.push_back(player.first);
    }
    return ids;
}

bool GameRoom::isInRace() {
    std::lock_guard<std::mutex> lock(mtx);
    return state == RoomState::IN_RACE;
}

GameRoom::~GameRoom() {
    try {
        stopAllPlayers();
    } catch (...) {}
}
