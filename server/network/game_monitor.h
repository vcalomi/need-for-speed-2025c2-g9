#ifndef GAME_MONITOR_H
#define GAME_MONITOR_H

#include <functional>
#include <map>
#include <mutex>
#include <vector>

#include "../../common/Dto/dto.h"
#include "../../common/car_config.h"
#include "../../common/queue.h"

#include "game_room.h"

class GameMonitor {
private:
    mutable std::mutex mtx;
    std::map<int, std::shared_ptr<GameRoom>> clientToRoom;
    std::map<int, std::string> clientUsernames;
    std::map<std::string, CarConfig> availableCars;
    std::map<std::string, std::shared_ptr<GameRoom>> activeGames;

public:
    GameMonitor();
    bool createGameRoom(const std::string& roomName, int hostId,
                        Player* hostPlayer, int maxPlayers = 8);
    bool joinGameRoom(const std::string& roomName, int clientId,
                      Player* clientPlayer);
    std::vector<std::string> getAvailableRooms();
    bool startGameByClientId(int clientId);
    bool isGameStartedByClient(int clientId);
    Queue<std::shared_ptr<Dto>>& getGameQueueForClient(int clientId);
    bool chooseCarByClientId(int clientId, const CarConfig& car);
    std::vector<std::string> getPlayersInRoomByClient(int clientId);
    bool setUsername(int clientId, const std::string& username);
    std::string getUsername(int clientId) const;
    std::shared_ptr<GameRoom> getRoomByClient(int clientId);
    void removeClient(int clientId);
    void closeAll();
    ~GameMonitor();
};

#endif
