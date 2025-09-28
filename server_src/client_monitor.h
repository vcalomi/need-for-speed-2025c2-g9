#ifndef CLIENT_MONITOR_H
#define CLIENT_MONITOR_H

#include <vector>

#include "../common_src/queue.h"

class ClientMonitor {
private:
    std::mutex mtx;
    std::vector<Queue<std::vector<uint8_t>>*> clientQueues;

public:
    ClientMonitor();
    void addQueue(Queue<std::vector<uint8_t>>* queue);
    void removeQueue(const Queue<std::vector<uint8_t>>* queue);
    void broadcast(const std::vector<uint8_t>& message);
};

#endif
