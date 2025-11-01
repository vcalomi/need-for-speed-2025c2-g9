#include "game_lobby.h"

#include <iostream>
#include <vector>

#include "../common/queue.h"

GameLobby::GameLobby() {}

bool GameLobby::createGameRoom(const std::string& roomName, int hostId, Queue<std::shared_ptr<Dto>>& hostQueue) {
    std::lock_guard<std::mutex> lock(mtx);

    GameRoom* newRoom = new GameRoom(roomName, hostId);
    activeGames[roomName] = newRoom;
    clientToRoom[hostId] = newRoom;
    newRoom->addPlayer(hostId, hostQueue);   
    return true;
}

bool GameLobby::joinGameRoom(const std::string& roomName, int clientId, Queue<std::shared_ptr<Dto>>& clientQueue) {
    std::lock_guard<std::mutex> lock(mtx);

    if (!activeGames.count(roomName)) {
        return false;
    }
    
    GameRoom* room = activeGames[roomName];
    if (!room->canJoin()) {
        return false;
    }
    
    room->addPlayer(clientId, clientQueue);
    clientToRoom[clientId] = room;
    return true;
}

std::vector<std::string> GameLobby::getAvailableRooms() {
    std::lock_guard<std::mutex> lock(mtx);

    std::vector<std::string> available;
    
    for (const auto& [roomName, gameRoom] : activeGames) {
        if (gameRoom->canJoin()) { 
            available.push_back(roomName);
        }
    }
    return available;
}

bool GameLobby::startGameByClientId(int clientId) {
    std::lock_guard<std::mutex> lock(mtx);
    
    if (!clientToRoom.count(clientId)) {
        return false; // Cliente no está en ninguna sala
    }

    GameRoom* room = clientToRoom[clientId];
    if (!room->isHost(clientId)) {
        return false;  // No es el host
    }
    
    bool ok = room->startGame();
    if (!ok) return false;

    std::vector<std::function<void()>> callbacks;
    callbacks.reserve(clientToRoom.size());
    for (const auto& entry : clientToRoom) {
        if (entry.second == room) {
            auto it = startNotifiers.find(entry.first);
            if (it != startNotifiers.end()) callbacks.push_back(it->second);
        }
    }

    for (auto& cb : callbacks) {
        if (cb) cb();
    }
    return true;
}

bool GameLobby::chooseCarByClientId(int clientId, const CarConfig& car) {
    std::lock_guard<std::mutex> lock(mtx);
    
    if (!clientToRoom.count(clientId)) {
        return false; // Cliente no está en ninguna sala
    }
    GameRoom* room = clientToRoom[clientId];
    return room->chooseCar(clientId, car);
}

Queue<std::shared_ptr<Dto>>& GameLobby::getGameQueueForClient(int clientId) {
    std::lock_guard<std::mutex> lock(mtx);

    if (!clientToRoom.count(clientId)) {
        throw std::runtime_error("Client not in any room");
    }
    GameRoom* room = clientToRoom[clientId];
    return room->getGameQueue();
}

GameLobby::~GameLobby() {
    for (auto& pair : activeGames) {
        delete pair.second;
    }
}

std::vector<std::string> GameLobby::getPlayersInRoomByClient(int clientId) {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<std::string> result;
    if (!clientToRoom.count(clientId)) return result;
    GameRoom* room = clientToRoom[clientId];
    auto ids = room->getPlayerIds();
    result.reserve(ids.size());
    for (int id : ids) {
        auto it = clientUsernames.find(id);
        if (it != clientUsernames.end()) {
            result.push_back(it->second);
        } else {
            result.push_back(std::to_string(id));
        }
    }
    return result;
}

void GameLobby::setUsername(int clientId, const std::string& username) {
    std::lock_guard<std::mutex> lock(mtx);
    clientUsernames[clientId] = username;
}

std::string GameLobby::getUsername(int clientId) const {
    auto it = clientUsernames.find(clientId);
    if (it != clientUsernames.end()) return it->second;
    return std::to_string(clientId);
}

bool GameLobby::isGameStartedByClient(int clientId) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!clientToRoom.count(clientId)) return false;
    GameRoom* room = clientToRoom[clientId];
    return room->isInRace();
}

void GameLobby::registerStartNotifier(int clientId, std::function<void()> notifier) {
    std::lock_guard<std::mutex> lock(mtx);
    startNotifiers[clientId] = std::move(notifier);
}
