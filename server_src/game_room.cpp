#include "game_room.h"
#include <iostream>
#include <algorithm>

#include "../common_src/queue.h"

GameRoom::GameRoom(const std::string& roomName, int hostId) : 
    roomName(roomName),
    hostId(hostId),
    state(RoomState::WAITING_FOR_PLAYERS),
    gameQueue(),
    gameLoop(gameQueue) {}

bool GameRoom::addPlayer(int clientId, Queue<NitroMessage>& senderQueue) {
    std::lock_guard<std::mutex> lock(mtx);
    
    if (!canJoin()) return false;

    players[clientId] = &senderQueue;
    return true;
}

bool GameRoom::startGame() {
    std::lock_guard<std::mutex> lock(mtx);
    
    if (state != RoomState::WAITING_FOR_PLAYERS || players.size() < 1) {
        return false;
    }

    // state = RoomState::IN_GAME;
    gameLoop.start();
    return true;
}

bool GameRoom::chooseCar(int clientId, const CarConfig& car) {
    std::lock_guard<std::mutex> lock(mtx);
    
    // if (state != RoomState::IN_GAME) return false;

    // Verificar que el jugador existe
    if (std::find(players.begin(), players.end(), clientId) == players.end()) {
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

Queue<ClientCommand>& GameRoom::getGameQueue() { return gameQueue; }

GameRoom::~GameRoom() {}