#include "game_lobby.h"

#include <iostream>
#include <vector>
#include <algorithm>

#include "../common_src/queue.h"

GameLobby::GameLobby() : lobbyOpen(true) {}

void GameLobby::registerClient(Socket socket, int clientId) {
    if (!lobbyOpen) {
        socket.close();
        return;
    }

    clientSockets[clientId] = std::move(socket);
}

void GameLobby::processClients() {
    for (auto& [clientId, socket] : clientSockets) {
        try {
            ServerProtocol protocol(socket);

            ActionCode action = protocol.receiveActionCode();
            switch (action) {
                case ActionCode::CREATE_ROOM: {
                    std::string roomName = protocol.receiveRoomName(); // o creo ROOMCODE
                    if (createGameRoom(roomName, clientId)) {
                        // protocol.sendMsg({ActionCode::ROOM_CREATED});
                    } else {
                        // protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
                    }
                    break;
                }
                    
                case ActionCode::JOIN_ROOM: {
                    std::string roomName = protocol.receiveRoomName();
                    if (joinGameRoom(roomName, clientId)) {
                        // protocol.sendMsg({ActionCode::JOIN_OK});
                        // state = ClientState::IN_GAME_ROOM;
                    } else {
                        // protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
                    }
                    break;
                }
                    
                case ActionCode::START_GAME: {
                    bool started = startGameByClientId(clientId);
                    if (!started) {
                        // protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
                    }
                    break;
                }

                case ActionCode::CHOOSE_CAR: {
                    CarConfig car = protocol.receiveCarConfig();
                    if (chooseCarByClientId(clientId, car)) {
                        // protocol.sendMsg({ActionCode::CHOOSE_CAR_OK});
                    } else {
                        // protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
                    }
                    break;
                    }
                    
                default:
                    // protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
                    break;
            }
        } catch (const SocketClosed& e) {
        }
    }
    reapClients();
}

bool GameLobby::createGameRoom(const std::string& roomName, int hostId) {
    std::lock_guard<std::mutex> lock(mtx);

    if (activeGames.count(roomName)) {
        return false; // si ya existe ese nombre
    }

    GameRoom* newRoom = new GameRoom(roomName, hostId);
    activeGames[roomName] = newRoom;
    clientToRoom[hostId] = newRoom;
    if (clientHandlers.count(hostId)) {
        Socket gameSocket = std::move(clientSockets[hostId]);
        clientSockets.erase(hostId);
        
        ClientHandler* handler = new ClientHandler(std::move(gameSocket));
        clientHandlers[hostId] = handler;
        newRoom->addPlayer(hostId, handler);
    }
    return true;
}

bool GameLobby::joinGameRoom(const std::string& roomName, int clientId) {
    std::lock_guard<std::mutex> lock(mtx);

    if (!activeGames.count(roomName)) {
        return false;
    }
    
    GameRoom* room = activeGames[roomName];
    if (!room->canJoin()) {
        return false;
    }
    
    if (clientHandlers.count(clientId)) {
        Socket gameSocket = std::move(clientSockets[clientId]);
        clientSockets.erase(clientId);
        
        ClientHandler* handler = new ClientHandler(std::move(gameSocket));
        clientHandlers[clientId] = handler;

        room->addPlayer(clientId, clientHandlers[clientId]);
        clientToRoom[clientId] = room;
        return true;
    }
    return false;
}

void GameLobby::sendAvailableRooms(ServerProtocol& protocol) {
    std::lock_guard<std::mutex> lock(mtx);

    std::vector<std::string> available;
    
    for (const auto& [roomName, gameRoom] : activeGames) {
        if (gameRoom->canJoin()) { 
            available.push_back(roomName);
        }
    }
    protocol.sendRoomList(available);
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

void GameLobby::reapClients() {
   for (auto it = clientHandlers.begin(); it != clientHandlers.end();) {
    auto& [clientId, handler] = *it;
    if (handler->is_alive()) {
        ++it;
        continue;
    }
    
    handler->join();
    if (clientToRoom.count(clientId)) {
        clientToRoom[clientId]->removePlayer(clientId);
        clientToRoom.erase(clientId);
    }
    delete handler;
    it = clientHandlers.erase(it);
   }
}

void GameLobby::clearClients() {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = clientHandlers.begin();
    while (it != clientHandlers.end()) {
        auto handler = it->second;
        handler->stop();
        handler->join();
        delete handler;
        it = clientHandlers.erase(it);
    }

    for (auto& [id, socket] : clientSockets) socket.close();
    clientSockets.clear();

    for (auto& [name, room] : activeGames) delete room;
    activeGames.clear();
    clientToRoom.clear();
}

GameLobby::~GameLobby() {
    for (auto& pair : activeGames) {
        delete pair.second;
    }
}
