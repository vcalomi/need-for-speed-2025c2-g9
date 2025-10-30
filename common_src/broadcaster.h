#ifndef BROADCASTER_H
#define BROADCASTER_H

#include <vector>
#include <mutex>
#include "../common_src/queue.h"
#include "Dto/dto.h"

class Broadcaster {
private:
    std::vector<Queue<Dto>*> queues;
    std::mutex mtx;

public:
    void addQueue(Queue<Dto>* queue);
    void removeQueue(Queue<Dto>* queue);
    void broadcast(const Dto& message);
};

#endif