#ifndef GAME_ROOM_H
#define GAME_ROOM_H

#include <map>
#include <string>

#include "../common_src/queue.h"
#include <atomic>
#include "gameloop.h"
#include "../common_src/client_command.h"
#include "../common_src/nitro_message.h"

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
    std::map<int, Queue<NitroMessage>*> players;
    std::map<int, CarConfig> chosenCars;
    RoomState state;
    Queue<ClientCommand> gameQueue;  // Para comandos de juego
    GameLoop gameLoop;
    // ConfigCarrera config;
    // GameLoop& gameLoop;
    // resultados de las races
    // int currentRace;
    // CANT DE RACES

public:
    GameRoom(const std::string& roomName, int hostId);
    
    // gestión de jugadores
    bool addPlayer(int clientId, Queue<NitroMessage>& senderQueue);
    // void removePlayer(int clientId);

    // control del juego
    bool startGame();
    // void finishRace();
    // void startNextRace();
    // void endGame();

    // elección de autos o upgrade
    bool chooseCar(int clientId, const CarConfig& car);
    // bool upgradeCar(int clientId, const CarUpgrade& upgrade);

    bool canJoin() const;
    bool isHost(int clientId) const;
    Queue<ClientCommand>& getGameQueue();
    
    ~GameRoom();
};

#endif
