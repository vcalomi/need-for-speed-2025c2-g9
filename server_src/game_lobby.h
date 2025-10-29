#ifndef GAME_LOBBY_H
#define GAME_LOBBY_H

#include <vector>
#include <map>

#include "../common_src/queue.h"
#include "game_room.h"
#include "client_handler.h"

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
    
    public:
    GameLobby();
    void registerClientHandler(int clientId, ClientHandler* handler);
    bool createGameRoom(const std::string& roomName, int hostId, Queue<NitroMessage>& hostQueue);
    bool joinGameRoom(const std::string& roomName, int clientId, Queue<NitroMessage>& clientQueue);
    std::vector<std::string> getAvailableRooms();
    bool startGameByClientId(int clientId);
    Queue<ClientCommand>& getGameQueueForClient(int clientId);
    bool chooseCarByClientId(int clientId, const CarConfig& car);
    ~GameLobby();
};

#endif
