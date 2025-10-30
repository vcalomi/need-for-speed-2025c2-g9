#ifndef GAME_ROOM_H
#define GAME_ROOM_H

#include <map>
#include <string>

#include "../common_src/queue.h"
#include <atomic>
#include "gameloop.h"
#include "../common_src/client_command.h"
#include "../common_src/Dto/dto.h"
#include "../common_src/broadcaster.h"

/*
    Maneja el estado y los jugadores de cada sala
*/

enum class RoomState {
    WAITING_FOR_PLAYERS,
    CHOOSING_CARS,
    IN_RACE,
    RACE_FINISHED
};

class GameRoom {
private:
    std::mutex mtx;
    std::string roomName;
    int hostId;
    // clientId y su queue
    std::map<int, Queue<std::shared_ptr<Dto>>*> players;
    std::map<int, CarConfig> chosenCars;
    RoomState state;
    Queue<std::shared_ptr<Dto>> gameQueue;  // Para comandos de juego
    // GameLoop gameLoop;
    // ConfigCarrera config;
    // GameLoop& gameLoop;
    Broadcaster broadcaster;

public:
    GameRoom(const std::string& roomName, int hostId);
    
    // gestión de jugadores
    bool addPlayer(int clientId, Queue<std::shared_ptr<Dto>>& senderQueue);
    void removePlayer(int clientId);
    bool startGame();
    bool chooseCar(int clientId, const CarConfig& car);
    bool canJoin() const;
    bool isHost(int clientId) const;
    Queue<std::shared_ptr<Dto>>& getGameQueue();
    
    ~GameRoom();
};

#endif
