#include "game_room.h"
#include <iostream>
#include <algorithm>

#include "../common_src/queue.h"
#include "client_handler.h"

GameRoom::GameRoom(const std::string& roomName, int hostId) : 
    roomName(roomName),
    hostId(hostId),
    state(RoomState::WAITING_FOR_PLAYERS),
    gameQueue() {}
    // gameLoop(gameQueue) {}

bool GameRoom::addPlayer(int clientId, ClientHandler* client) {
    std::lock_guard<std::mutex> lock(mtx);

    if (!canJoin()) return false;

    players[clientId] = client;
    broadcaster.addQueue(&client->senderQueue);
    return true;
}

bool GameRoom::removePlayer(int clientId) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = players.find(clientId);
    broadcaster.removeQueue(&it->second->senderQueue);
    if (it == players.end()) return false;
    players.erase(it);
    return true;
}

bool GameRoom::startGame() {
    std::lock_guard<std::mutex> lock(mtx);
    
    if (state != RoomState::WAITING_FOR_PLAYERS || players.size() < 1) {
        return false;
    }

    state = RoomState::IN_RACE;
    for (const auto& [playerId, handler] : players) {
        if (handler) {
            handler->start(gameQueue);
        }
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

bool GameRoom::canJoin() const {
    return state == RoomState::WAITING_FOR_PLAYERS && players.size() < 8;
}

bool GameRoom::isHost(int clientId) const {
    return hostId == clientId;
}

void GameRoom::broadcastToAll(const Dto& message) {
    broadcaster.broadcast(message);
}

GameRoom::~GameRoom() {}