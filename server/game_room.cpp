#include "game_room.h"
#include <iostream>
#include <algorithm>

#include "../common/queue.h"
#include "../common/Dto/vehicle.h"

GameRoom::GameRoom(const std::string& roomName, int hostId) : 
    roomName(roomName),
    hostId(hostId),
    state(RoomState::WAITING_FOR_PLAYERS),
    gameQueue(),
    broadcaster() {}
    // gameLoop(gameQueue) {}

bool GameRoom::addPlayer(int clientId, Queue<std::shared_ptr<Dto>>& senderQueue) {
    std::lock_guard<std::mutex> lock(mtx);
    
    if (!canJoin()) return false;

    players[clientId] = &senderQueue;
    broadcaster.addQueue(&senderQueue);
    return true;
}

bool GameRoom::startGame() {
    std::lock_guard<std::mutex> lock(mtx);
    
    if (state != RoomState::WAITING_FOR_PLAYERS || players.size() < 1) {
        return false;
    }

    state = RoomState::IN_RACE;

    uint8_t idx = 0;
    for (const auto& entry : players) {
        uint8_t id = static_cast<uint8_t>(entry.first % 255);
        auto dto = std::make_shared<VehicleDto>(id, 1.0f + idx, 0.0f, 0.0f);;
        std::cout << "CREATED VehicleDto: id=" << (int)dto->id 
                  << " x=" << dto->x << " y=" << dto->y << std::endl;
        
        broadcaster.broadcast(dto);
        idx++;
    }
    // gameLoop.start();
    return true;
}

bool GameRoom::chooseCar(int clientId, const CarConfig& car) {
    std::lock_guard<std::mutex> lock(mtx);
    
    // if (state != RoomState::IN_GAME) return false;

    // Verificar que el jugador existe
    if (players.find(clientId) == players.end()) {
        return false;
    }
    
    chosenCars[clientId] = car;
    return true;
}

// void removePlayer(int clientId) {

// }

bool GameRoom::canJoin() const {
    return state == RoomState::WAITING_FOR_PLAYERS && players.size() < 8;
}

bool GameRoom::isHost(int clientId) const {
    return hostId == clientId;
}

Queue<std::shared_ptr<Dto>>& GameRoom::getGameQueue() { return gameQueue; }

std::vector<int> GameRoom::getPlayerIds() {
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<int> ids;
    ids.reserve(players.size());
    for (const auto& player : players) {
        ids.push_back(player.first);
    }
    return ids;
}

bool GameRoom::isInRace() {
    std::lock_guard<std::mutex> lock(mtx);
    return state == RoomState::IN_RACE;
}

GameRoom::~GameRoom() {}