#ifndef BROADCASTER_H
#define BROADCASTER_H

#include <vector>
#include <mutex>
#include "../common_src/queue.h"
#include "Dto/dto.h"

class Broadcaster {
private:
    std::vector<Queue<std::shared_ptr<Dto>>*> queues;
    std::mutex mtx;

public:
    void addQueue(Queue<std::shared_ptr<Dto>>* queue);
    void removeQueue(Queue<std::shared_ptr<Dto>>* queue);
    void broadcast(std::shared_ptr<Dto> dto);
};

#endif