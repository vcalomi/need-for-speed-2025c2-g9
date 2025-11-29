#include "game_room.h"

#include <algorithm>
#include <iostream>

#include "../../common/Dto/vehicle.h"
#include "../../common/Dto/player_left.h"
#include "../../common/Dto/lobby_room_state.h"

GameRoom::GameRoom(const std::string& roomName, int hostId, int maxPlayers)
    : roomName(roomName), hostId(hostId), maxPlayers_(maxPlayers), 
      state(RoomState::WAITING_FOR_PLAYERS), gameQueue(),
      broadcaster(), gameLoop(gameQueue, chosenCars, playerUsernames, broadcaster, maxPlayers, selectedMaps) {}

bool GameRoom::addPlayer(int clientId, Player* player) {
    std::lock_guard<std::mutex> lock(mtx);
    
    if (!canJoin()) {
        return false;
    }

    players[clientId] = player;
    broadcaster.addQueue(&player->getSendQueue());
    return true;
}

bool GameRoom::setPlayerUsername(int clientId, const std::string& username) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& [id, existingUsername] : playerUsernames) {
        if (existingUsername == username && id != clientId) {
            return false;
        }
    }
    playerUsernames[clientId] = username;
    return true;
}

bool GameRoom::chooseCar(int clientId, const CarConfig& car) {
    std::lock_guard<std::mutex> lock(mtx);
    if (players.find(clientId) == players.end()) return false;
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

    auto playerIt = players.find(clientId);
    if (playerIt != players.end()) {
        broadcaster.removeQueue(&playerIt->second->getSendQueue());
        players.erase(playerIt);
    }
    
    playerUsernames.erase(clientId);
    chosenCars.erase(clientId);
    
    if (!leftUsername.empty()) {
        auto dto = std::make_shared<PlayerLeftDto>(leftUsername);
        broadcaster.broadcast(dto);
    }
}

bool GameRoom::startGame() {
    std::lock_guard<std::mutex> lock(mtx);
    
    if (state != RoomState::WAITING_FOR_PLAYERS) {
        return false;
    }

    for (const auto& [id, player] : players) {
        if (chosenCars.find(id) == chosenCars.end()) {
            chosenCars[id] = CarConfig{"jeep"};
        }
    }

    state = RoomState::IN_RACE;
    return true;
}

void GameRoom::startGameForPlayer(int clientId) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = players.find(clientId);
    if (it != players.end()) {
        if (it->second) {
            if (!it->second->isGameActive()) {
                it->second->beginGame(gameQueue);
                checkAndStartGameLoop();
            }
        }
    }
}

void GameRoom::checkAndStartGameLoop() {
    bool allInGame = true;
    for (const auto& [id, player] : players) {
        if (!player->isGameActive()) {
            allInGame = false;
            break;
        }
    }

    if (allInGame) {
        gameLoop.start();
    }
}

bool GameRoom::canJoin() const {
    bool can = state == RoomState::WAITING_FOR_PLAYERS &&
           static_cast<int>(players.size()) < maxPlayers_;
    
    return can;
}

bool GameRoom::isHost(int clientId) const { 
    return hostId == clientId; 
}

std::string GameRoom::getCarType(int clientId) const {
    auto it = chosenCars.find(clientId);
    if (it != chosenCars.end()) {
        return it->second.carType;
    }
    return "jeep";
}

std::vector<int> GameRoom::getPlayerIds() {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<int> ids;
    ids.reserve(players.size());
    for (const auto& player : players) {
        ids.push_back(player.first);
    }
    return ids;
}

void GameRoom::setSelectedMaps(const std::vector<uint8_t>& mapIds) {
    std::lock_guard<std::mutex> lock(mtx);
    selectedMaps.clear();
    for (uint8_t mapCode : mapIds) {
        selectedMaps.push_back(mapCodeToString(static_cast<MapCode>(mapCode)));
    }
}

void GameRoom::stopAllPlayers() {
    std::lock_guard<std::mutex> lock(mtx);

    try { 
        gameQueue.close(); 
    } catch (...) {}

    for (auto& [id, player] : players) {
        if (player) {
            try {
                player->stopGame();
            } catch (...) {}
        }
    }
    players.clear();

    try {
        gameLoop.stop();
        if (gameLoop.is_alive()) {
            gameLoop.join();
        }
    } catch (...) {}
}

GameRoom::~GameRoom() {
    try {
        stopAllPlayers();
    } catch (...) {}
}
