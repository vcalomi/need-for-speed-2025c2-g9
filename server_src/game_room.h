#ifndef GAME_ROOM_H
#define GAME_ROOM_H

#include <map>
#include <string>

#include "client_handler.h"
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
    // clientId y su handler
    std::map<int, ClientHandler*> players;
    std::map<int, CarConfig> chosenCars;
    RoomState state;
    Queue<Dto> gameQueue;  // Para comandos de juego
    // GameLoop gameLoop;
    Broadcaster broadcaster;
    // ConfigCarrera config;
    // GameLoop& gameLoop;

public:
    GameRoom(const std::string& roomName, int hostId);
    bool addPlayer(int clientId, ClientHandler* handler);
    bool removePlayer(int clientId);
    bool startGame();
    bool chooseCar(int clientId, const CarConfig& car);
    void broadcastToAll(const Dto& message);
    bool canJoin() const;
    bool isHost(int clientId) const;
    const std::map<int, ClientHandler*>& getPlayers() const { return players; }
    ~GameRoom();
};

#endif
