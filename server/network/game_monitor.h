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
    bool startGameRoom(int clientId);
    bool isGameStarted(int clientId);
    Queue<std::shared_ptr<Dto>>& getGameQueue(int clientId);
    bool choosePlayerCar(int clientId, const CarConfig& car);
    std::vector<std::string> getPlayersInRoom(int clientId);
    bool setUsername(int clientId, const std::string& username);
    std::shared_ptr<GameRoom> getRoom(int clientId);
    void removeClient(int clientId);
    void closeAll();
    ~GameMonitor();
};

#endif
