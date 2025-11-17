#ifndef GAME_MONITOR_H
#define GAME_MONITOR_H

#include <functional>
#include <map>
#include <vector>

#include "../../common/Dto/dto.h"
#include "../../common/queue.h"

#include "game_room.h"

/*
    (Monitor) Gestiona las partidas
*/

class GameMonitor {
private:
    std::mutex mtx;
    // clienteId y GameRoom
    std::map<int, std::shared_ptr<GameRoom>> clientToRoom;
    std::map<int, std::string> clientUsernames;
    std::map<std::string, CarConfig> availableCars;
    std::map<std::string, std::shared_ptr<GameRoom>> activeGames;
    std::map<int, std::function<void(std::shared_ptr<GameRoom>)>> startNotifiers;
    std::map<int, bool> pendingGameStarts;

public:
    GameMonitor();
    bool createGameRoom(const std::string& roomName, int hostId,
                        Queue<std::shared_ptr<Dto>>& hostQueue, int maxPlayers = 8);
    bool joinGameRoom(const std::string& roomName, int clientId,
                      Queue<std::shared_ptr<Dto>>& clientQueue);
    std::vector<std::string> getAvailableRooms();
    bool startGameByClientId(int clientId);
    Queue<std::shared_ptr<Dto>>& getGameQueueForClient(int clientId);
    bool chooseCarByClientId(int clientId, const CarConfig& car);
    std::vector<std::string> getPlayersInRoomByClient(int clientId);
    bool setUsername(int clientId, const std::string& username);
    std::string getUsername(int clientId) const;
    bool isGameStartedByClient(int clientId);
    void registerStartNotifier(int clientId,
                               std::function<void(std::shared_ptr<GameRoom>)> notifier);
    std::shared_ptr<GameRoom> getRoomByClient(int clientId);
    void removeClient(int clientId);
    ~GameMonitor();
};

#endif
#include <memory>
#include <string>
