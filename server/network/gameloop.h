#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <map>
#include <vector>

#include "../../common/Dto/dto.h"
#include "../../common/common_codes.h"
#include "../../common/queue.h"
#include "../../common/thread.h"
#include "../../common/broadcaster.h"
#include "../../common/car_config.h"
#include "../LevelSetup.h"


class GameLoop: public Thread {
private:
    Queue<std::shared_ptr<Dto>>& gameLoopQueue;
    std::map<int, CarConfig>& chosenCars_;
    Broadcaster& broadcaster_;
    LevelSetup setup;

public:
    explicit GameLoop( Queue<std::shared_ptr<Dto>>& gameLoopQueue, std::map<int, CarConfig>& chosenCars, Broadcaster& broadcaster);

    void run() override;
    void processCommands();
    void simulateGame();

    ~GameLoop();
};

#endif
