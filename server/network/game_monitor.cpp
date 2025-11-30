#include "game_monitor.h"

#include <iostream>
#include <utility>
#include <vector>

#include "../../common/queue.h"

GameMonitor::GameMonitor() {}

std::shared_ptr<GameRoom> GameMonitor::getRoom(int clientId) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!clientToRoom.count(clientId)) {
        return nullptr;
    }
    return clientToRoom[clientId];
}

bool GameMonitor::createGameRoom(const std::string& roomName, int hostId,
                                 Player* hostPlayer, int maxPlayers) {
    std::lock_guard<std::mutex> lock(mtx);

    if (activeGames.count(roomName)) {
        return false;
    }

    auto newRoom = std::make_shared<GameRoom>(roomName, hostId, maxPlayers);
    activeGames[roomName] = newRoom;
    clientToRoom[hostId] = newRoom;

    newRoom->addPlayer(hostId, hostPlayer);
    
    auto it = clientUsernames.find(hostId);
    if (it != clientUsernames.end()) {
        newRoom->setPlayerUsername(hostId, it->second);
    }

    return true;
}

bool GameMonitor::joinGameRoom(const std::string& roomName, int clientId,
                               Player* clientPlayer) {
    std::lock_guard<std::mutex> lock(mtx);

    if (!activeGames.count(roomName)) {
        return false;
    }

    std::shared_ptr<GameRoom> room = activeGames[roomName];
    if (!room->canJoin()) {
        return false;
    }

    room->addPlayer(clientId, clientPlayer);
    clientToRoom[clientId] = room;
    
    auto it = clientUsernames.find(clientId);
    if (it != clientUsernames.end()) {
        room->setPlayerUsername(clientId, it->second);
    }

    return true;
}

std::vector<std::string> GameMonitor::getAvailableRooms() {
    std::lock_guard<std::mutex> lock(mtx);

    std::vector<std::string> available;

    for (const auto& [roomName, gameRoom]: activeGames) {
        if (gameRoom->canJoin()) {
            available.push_back(roomName);
        }
    }
    return available;
}

bool GameMonitor::startGameRoom(int clientId) {
    std::lock_guard<std::mutex> lock(mtx);

    if (!clientToRoom.count(clientId))
        return false;
    
    auto room = clientToRoom[clientId];
    if (!room->isHost(clientId))
        return false;

    return room->startRace();
}

bool GameMonitor::choosePlayerCar(int clientId, const CarConfig& car) {
    std::lock_guard<std::mutex> lock(mtx);

    if (!clientToRoom.count(clientId)) {
        return false;
    }
    std::shared_ptr<GameRoom> room = clientToRoom[clientId];
    return room->chooseCar(clientId, car);
}

Queue<std::shared_ptr<Dto>>& GameMonitor::getGameQueue(int clientId) {
    std::lock_guard<std::mutex> lock(mtx);

    if (!clientToRoom.count(clientId)) {
        throw std::runtime_error("Client not in any room");
    }
    std::shared_ptr<GameRoom> room = clientToRoom[clientId];
    return room->getGameQueue();
}

std::vector<std::string> GameMonitor::getPlayersInRoom(int clientId) {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<std::string> result;
    if (!clientToRoom.count(clientId))
        return result;

    std::shared_ptr<GameRoom> room = clientToRoom[clientId];
    auto ids = room->getPlayerIds();
    result.reserve(ids.size());
    for (int id: ids) {
        auto it = clientUsernames.find(id);
        result.push_back(it != clientUsernames.end() ? it->second : std::to_string(id));
    }
    return result;
}

bool GameMonitor::setUsername(int clientId, const std::string& username) {
    std::lock_guard<std::mutex> lock(mtx);
    if (clientToRoom.count(clientId)) {
        auto room = clientToRoom[clientId];
        if (!room->setPlayerUsername(clientId, username)) {
            return false;
        }
    }
    clientUsernames[clientId] = username;
    return true;
}

void GameMonitor::removeClient(int clientId) {
    std::lock_guard<std::mutex> lock(mtx);
    auto roomIt = clientToRoom.find(clientId);
    if (roomIt != clientToRoom.end()) {
        std::shared_ptr<GameRoom> room = roomIt->second;
        room->removePlayer(clientId);
        clientToRoom.erase(roomIt);
    }
    clientUsernames.erase(clientId);
}

void GameMonitor::closeAll() {
    std::lock_guard<std::mutex> lock(mtx);
    
    for (auto& [roomName, room] : activeGames) {
        try {
            room->stopAllPlayers();
        } catch (...) {}
    }
    
    clientToRoom.clear();
    clientUsernames.clear();
    activeGames.clear();
}

GameMonitor::~GameMonitor() {}
