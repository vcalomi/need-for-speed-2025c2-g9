#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <map>
#include <vector>

#include "../common_src/Dto/dto.h"
#include "../common_src/common_codes.h"
#include "../common_src/queue.h"
#include "../common_src/thread.h"
#include "game_room.h"

/*
    Maneja el juego
    FALTA JOIN
*/

class GameLoop: public Thread {
private:
    // std::map<int, CarConfig>& cars;
    Queue<Dto>& gameLoopQueue;

public:
    GameLoop(Queue<Dto>& gameLoopQueue);
    void run() override;
    void processCommands();
    void simulateGame();
    
    // logica del juego
    // checkpoints
    ~GameLoop();
};

#endif
