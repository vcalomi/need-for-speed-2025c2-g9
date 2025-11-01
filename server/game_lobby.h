#ifndef GAME_LOBBY_H
#define GAME_LOBBY_H

#include <vector>
#include <map>

#include "../common/queue.h"
#include "game_room.h"
#include "../common/Dto/dto.h"

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
    
    public:
    GameLobby();
    bool createGameRoom(const std::string& roomName, int hostId, Queue<std::shared_ptr<Dto>>& hostQueue);
    bool joinGameRoom(const std::string& roomName, int clientId, Queue<std::shared_ptr<Dto>>& clientQueue);
    std::vector<std::string> getAvailableRooms();
    bool startGameByClientId(int clientId);
    Queue<std::shared_ptr<Dto>>& getGameQueueForClient(int clientId);
    bool chooseCarByClientId(int clientId, const CarConfig& car);
    ~GameLobby();
};

#endif
