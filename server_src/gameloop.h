#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "../common_src/common_codes.h"
#include "../common_src/thread.h"
#include "../common_src/queue.h"
#include "client_monitor.h"
// #include "nitro_state.h"

class GameLoop : public Thread {
private:
    Queue<ActionCode>& commandQueue;    // Queue compartida (de los Receivers)
    ClientMonitor& clientMonitor;      // Para hacer broadcast
    uint16_t carsWithActiveNitro;
    bool nitroActive;
    int counterActive;
    std::atomic<bool> keep_running;

public:
    GameLoop(Queue<ActionCode>& commandQueue, ClientMonitor& clientMonitor);
    void run() override;
    void close();
    void broadcastNitroEvent(bool activated);
    void processCommands();
    void simulateGame();
    std::vector<uint8_t> createNitroMessage(uint16_t carsWithActiveNitro, bool activated);
    ~GameLoop();
};

#endif
