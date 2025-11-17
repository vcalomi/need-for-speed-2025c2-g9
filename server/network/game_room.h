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
/*
    Maneja el estado y los jugadores de cada sala
*/

enum class RoomState { WAITING_FOR_PLAYERS, CHOOSING_CARS, IN_RACE, RACE_FINISHED };

class GameRoom {
private:
    std::mutex mtx;
    std::string roomName;
    int hostId;
    // clientId y su queue
    std::map<int, Queue<std::shared_ptr<Dto>>*> players;
    std::map<int, std::string> playerUsernames;
    std::map<int, CarConfig> chosenCars;
    int maxPlayers_;
    RoomState state;

    Queue<std::shared_ptr<Dto>> gameQueue;  // Para comandos de juego
    Broadcaster broadcaster;
    GameLoop gameLoop;


public:
    GameRoom(const std::string& roomName, int hostId, int maxPlayers);

    // gestión de jugadores
    bool addPlayer(int clientId, Queue<std::shared_ptr<Dto>>& senderQueue);
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
    ~GameRoom();
};

#endif
