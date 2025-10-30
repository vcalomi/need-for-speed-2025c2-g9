#ifndef GAME_LOBBY_H
#define GAME_LOBBY_H

#include <vector>
#include <map>

#include "client_handler.h"
#include "../common_src/queue.h"
#include "game_room.h"
#include "../common_src/Dto/dto.h"
#include "server_protocol.h"

/*
    (Monitor) Gestiona las partidas
*/

class GameLobby {
private:
    std::mutex mtx;
    // clienteId y GameRoom
    std::map<int, GameRoom*> clientToRoom;
    std::map<std::string, CarConfig> availableCars;
    std::map<std::string, GameRoom*> activeGames;
    std::map<int, ClientHandler*> clientHandlers;
    std::map<int, Socket> clientSockets;
    std::atomic<bool> lobbyOpen{true};
    
public:
    GameLobby();
    void processClients();
    void registerClient(Socket socket, int clientId);
    bool createGameRoom(const std::string& roomName, int hostId);
    bool joinGameRoom(const std::string& roomName, int clientId);
    void sendAvailableRooms(ServerProtocol& protocol);
    bool startGameByClientId(int clientId);
    bool chooseCarByClientId(int clientId, const CarConfig& car);
    void reapClients();
    void clearClients();
    ~GameLobby();
};

#endif
