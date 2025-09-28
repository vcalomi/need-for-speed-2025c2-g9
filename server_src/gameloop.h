#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <map>
#include <vector>

#include "../common_src/client_command.h"
#include "../common_src/common_codes.h"
#include "../common_src/nitro_state.h"
#include "../common_src/queue.h"
#include "../common_src/thread.h"

#include "client_monitor.h"
class GameLoop: public Thread {
private:
    Queue<ClientCommand>& commandQueue;
    ClientMonitor& clientMonitor;
    std::atomic<bool> keep_running;
    std::map<int, NitroState> nitroStates;

public:
    GameLoop(Queue<ClientCommand>& commandQueue, ClientMonitor& clientMonitor);
    void run() override;
    void close();
    void broadcastNitroEvent(uint16_t carsWithActiveNitro, bool activated);
    void processCommands();
    void simulateGame();
    int countActiveNitro();
    void processNitroCommand(int clientId);
    std::vector<uint8_t> createNitroMessage(uint16_t carsWithActiveNitro, bool activated);
    ~GameLoop();
};

#endif
