#include "game_lobby.h"

#include <iostream>
#include <vector>

#include "../common_src/queue.h"

GameLobby::GameLobby() {}

void GameLobby::registerClientHandler(int clientId, ClientHandler* handler) {
    std::lock_guard<std::mutex> lock(mtx);
    clientHandlers[clientId] = handler;
}


bool GameLobby::createGameRoom(const std::string& roomName, int hostId, Queue<NitroMessage>& hostQueue) {
    std::lock_guard<std::mutex> lock(mtx);

    GameRoom* newRoom = new GameRoom(roomName, hostId);
    activeGames[roomName] = newRoom;
    clientToRoom[hostId] = newRoom;
    newRoom->addPlayer(hostId, hostQueue);
    return true;
}

bool GameLobby::joinGameRoom(const std::string& roomName, int clientId, Queue<NitroMessage>& clientQueue) {
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

    if (!room->startGame()) return false;
    
    // inciar threads sender/receiver de todos los jugadores de esa partida
    Queue<ClientCommand>& gameQueue = room->getGameQueue();
    for (const auto& [playerId, _] : room->getPlayers()) {
        if (clientHandlers.count(playerId)) {
            ClientHandler* handler = clientHandlers[playerId];
            handler->startGameThreads(gameQueue);
        }
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

Queue<ClientCommand>& GameLobby::getGameQueueForClient(int clientId) {
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
