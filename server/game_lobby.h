#ifndef GAME_LOBBY_H
#define GAME_LOBBY_H

#include <vector>
#include <map>
#include <functional>

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
    std::map<int, std::string> clientUsernames;
    std::map<std::string, CarConfig> availableCars;
    std::map<std::string, GameRoom*> activeGames;
    // clientId -> callback a invocar cuando la partida de su sala se inicia
    std::map<int, std::function<void()>> startNotifiers;
    
    public:
    GameLobby();
    bool createGameRoom(const std::string& roomName, int hostId, Queue<std::shared_ptr<Dto>>& hostQueue);
    bool joinGameRoom(const std::string& roomName, int clientId, Queue<std::shared_ptr<Dto>>& clientQueue);
    std::vector<std::string> getAvailableRooms();
    bool startGameByClientId(int clientId);
    Queue<std::shared_ptr<Dto>>& getGameQueueForClient(int clientId);
    bool chooseCarByClientId(int clientId, const CarConfig& car);
    std::vector<std::string> getPlayersInRoomByClient(int clientId);
    void setUsername(int clientId, const std::string& username);
    std::string getUsername(int clientId) const;
    bool isGameStartedByClient(int clientId);
    void registerStartNotifier(int clientId, std::function<void()> notifier);
    ~GameLobby();
};

#endif
