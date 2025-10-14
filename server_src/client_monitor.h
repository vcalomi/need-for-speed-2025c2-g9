#ifndef CLIENT_MONITOR_H
#define CLIENT_MONITOR_H

#include <vector>

#include "../common_src/nitro_message.h"
#include "../common_src/queue.h"

class ClientMonitor {
private:
    std::mutex mtx;
    std::vector<Queue<NitroMessage>*> clientQueues;

public:
    ClientMonitor();
    void addQueue(Queue<NitroMessage>* queue);
    void removeQueue(const Queue<NitroMessage>* queue);
    void broadcast(const NitroMessage& message);
};

#endif
