#include "game_monitor.h"

#include <iostream>
#include <utility>
#include <vector>

#include "../../common/queue.h"

GameMonitor::GameMonitor() {}

std::shared_ptr<GameRoom> GameMonitor::getRoomByClient(int clientId) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!clientToRoom.count(clientId))
        return nullptr;
    return clientToRoom[clientId];
}

bool GameMonitor::createGameRoom(const std::string& roomName, int hostId,
                                 Queue<std::shared_ptr<Dto>>& hostQueue, int maxPlayers) {
    std::lock_guard<std::mutex> lock(mtx);

    auto newRoom = std::make_shared<GameRoom>(roomName, hostId, maxPlayers);
    activeGames[roomName] = newRoom;
    clientToRoom[hostId] = newRoom;
    newRoom->addPlayer(hostId, hostQueue);
    return true;
}

bool GameMonitor::joinGameRoom(const std::string& roomName, int clientId,
                               Queue<std::shared_ptr<Dto>>& clientQueue) {
    std::lock_guard<std::mutex> lock(mtx);

    if (!activeGames.count(roomName)) {
        return false;
    }

    std::shared_ptr<GameRoom> room = activeGames[roomName];
    if (!room->canJoin()) {
        return false;
    }

    room->addPlayer(clientId, clientQueue);
    clientToRoom[clientId] = room;
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

bool GameMonitor::startGameByClientId(int clientId) {
    std::vector<std::pair<int, std::function<void(std::shared_ptr<GameRoom>)>>> callbacksToExecute;
    std::shared_ptr<GameRoom> targetRoom = nullptr;

    {
        std::lock_guard<std::mutex> lock(mtx);

        if (!clientToRoom.count(clientId))
            return false;
        auto room = clientToRoom[clientId];
        if (!room->isHost(clientId))
            return false;
        if (!room->startGame())
            return false;

        targetRoom = room;
        auto it = startNotifiers.find(clientId);
        if (it != startNotifiers.end()) {
            callbacksToExecute.push_back({clientId, it->second});
        }
    }

    for (auto& [cid, callback]: callbacksToExecute) {
        try {
            if (callback && targetRoom) {
                callback(targetRoom);
            }
        } catch (const std::exception& e) {
            std::cout << "Error in callback for client " << cid << ": " << e.what() << std::endl;
        }
    }

    return true;
}


bool GameMonitor::chooseCarByClientId(int clientId, const CarConfig& car) {
    std::lock_guard<std::mutex> lock(mtx);

    if (!clientToRoom.count(clientId)) {
        return false;  // Cliente no está en ninguna sala
    }
    std::shared_ptr<GameRoom> room = clientToRoom[clientId];
    return room->chooseCar(clientId, car);
}

Queue<std::shared_ptr<Dto>>& GameMonitor::getGameQueueForClient(int clientId) {
    std::lock_guard<std::mutex> lock(mtx);

    if (!clientToRoom.count(clientId)) {
        throw std::runtime_error("Client not in any room");
    }
    std::shared_ptr<GameRoom> room = clientToRoom[clientId];
    return room->getGameQueue();
}

std::vector<std::string> GameMonitor::getPlayersInRoomByClient(int clientId) {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<std::string> result;
    if (!clientToRoom.count(clientId))
        return result;

    std::shared_ptr<GameRoom> room = clientToRoom[clientId];
    auto ids = room->getPlayerIds();
    result.reserve(ids.size());
    result.push_back("maxPlayers:" + std::to_string(room->getMaxPlayers()));
    for (int id: ids) {
        auto it = clientUsernames.find(id);
        result.push_back(it != clientUsernames.end() ? it->second : std::to_string(id));
    }
    return result;
}

bool GameMonitor::setUsername(int clientId, const std::string& username) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& [id, name]: clientUsernames) {
        if (name == username && id != clientId) {
            return false;
        }
    }

    clientUsernames[clientId] = username;
    if (clientToRoom.count(clientId)) {
        auto room = clientToRoom[clientId];
        return room->setPlayerUsername(clientId, username);
    }

    return true;
}

std::string GameMonitor::getUsername(int clientId) const {
    auto it = clientUsernames.find(clientId);
    if (it != clientUsernames.end())
        return it->second;
    return std::to_string(clientId);
}

bool GameMonitor::isGameStartedByClient(int clientId) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!clientToRoom.count(clientId))
        return false;
    std::shared_ptr<GameRoom> room = clientToRoom[clientId];
    return room->isInRace();
}

void GameMonitor::registerStartNotifier(int clientId,
                                        std::function<void(std::shared_ptr<GameRoom>)> notifier) {
    std::lock_guard<std::mutex> lock(mtx);
    startNotifiers[clientId] = std::move(notifier);
}

GameMonitor::~GameMonitor() {}
