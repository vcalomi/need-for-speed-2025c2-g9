#ifndef GAME_ROOM_H
#define GAME_ROOM_H

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <mutex>

#include "player.h"
#include "../../common/broadcaster.h"
#include "gameloop.h"
#include "../../common/queue.h"
#include "../../common/Dto/player_left.h"
#include "../../common/car_config.h"
#include "../../common/common_codes.h"

enum class RoomState {
    WAITING_FOR_PLAYERS,
    IN_RACE
};

class GameRoom {
private:
    std::string roomName;
    int hostId;
    int maxPlayers_;
    RoomState state;
    mutable std::mutex mtx;
    
    std::map<int, Player*> players;
    std::map<int, std::string> playerUsernames;
    std::map<int, CarConfig> chosenCars;
    std::vector<std::string> selectedMaps;
    
    Queue<std::shared_ptr<Dto>> gameQueue;
    Broadcaster broadcaster;
    GameLoop gameLoop;

    void checkAndStartGameLoop();

public:
    GameRoom(const std::string& roomName, int hostId, int maxPlayers);
    
    bool addPlayer(int clientId, Player* player);
    bool setPlayerUsername(int clientId, const std::string& username);
    bool chooseCar(int clientId, const CarConfig& car);
    void removePlayer(int clientId);
    
    bool startGame();
    void startGameForPlayer(int clientId);
    
    bool canJoin() const;
    bool isHost(int clientId) const;
    bool isInRace() const { return state == RoomState::IN_RACE; }
    std::string getCarType(int clientId) const;
    std::vector<int> getPlayerIds();
    int getMaxPlayers() const { return maxPlayers_; }
    
    Queue<std::shared_ptr<Dto>>& getGameQueue() { return gameQueue; }
    void setSelectedMaps(const std::vector<uint8_t>& mapIds);
    
    void stopAllPlayers();
    ~GameRoom();
};

#endif
