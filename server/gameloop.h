#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <map>
#include <vector>

#include "../common/Dto/dto.h"
#include "../common/common_codes.h"
#include "../common/queue.h"
#include "../common/thread.h"
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
