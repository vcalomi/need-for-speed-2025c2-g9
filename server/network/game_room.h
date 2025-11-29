#include <memory>
#ifndef GAME_ROOM_H
#define GAME_ROOM_H

#include <atomic>
#include <map>
#include <string>
#include <vector>

#include "../../common/Dto/dto.h"
#include "../../common/broadcaster.h"
#include "../../common/car_config.h"
#include "../../common/queue.h"

#include "gameloop.h"
#include "player.h"
#include "player_info.h"

enum class RoomState { WAITING_FOR_PLAYERS, CHOOSING_CARS, IN_RACE, RACE_FINISHED };

class GameRoom {
private:
    std::mutex mtx;
    std::string roomName;
    int hostId;
    std::map<int, std::unique_ptr<Player>> gamePlayers;
    std::map<int, Player*> lobbyPlayers;
    std::map<int, std::string> playerUsernames;
    std::map<int, CarConfig> chosenCars;
    int maxPlayers_;
    RoomState state;
    std::vector<std::string> selectedMaps;

    Queue<std::shared_ptr<Dto>> gameQueue;
    Broadcaster broadcaster;
    GameLoop gameLoop;

public:
    GameRoom(const std::string& roomName, int hostId, int maxPlayers);

    // gestión de jugadores
    bool addPlayer(int clientId, Player* player);
    void addGamePlayer(std::shared_ptr<PlayerInfo> player_info);
    void removePlayer(int clientId);
    bool startGame();
    bool chooseCar(int clientId, const CarConfig& car);
    bool canJoin() const;
    bool isHost(int clientId) const;
    Queue<std::shared_ptr<Dto>>& getGameQueue();
    std::vector<int> getPlayerIds();
    bool isInRace();
    int getMaxPlayers() const { return maxPlayers_; }
    bool setPlayerUsername(int clientId, const std::string& username);
    std::string getRoomName() const { return roomName; }
    void setSelectedMaps(const std::vector<uint8_t>& mapIds);
    std::string getCarType(int clientId) const;
    bool allPlayersHaveCars() const;

    void startGameForAllPlayers();
    void startGameForPlayer(int clientId);
    void checkAndStartGameLoop();

    void stopAllPlayers();
    
    ~GameRoom();
};

#endif
