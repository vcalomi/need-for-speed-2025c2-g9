#include "broadcaster.h"

#include <algorithm>
#include <memory>

void Broadcaster::addQueue(Queue<std::shared_ptr<Dto>>* queue) {
    std::lock_guard<std::mutex> lock(mtx);
    queues.push_back(queue);
}

void Broadcaster::removeQueue(Queue<std::shared_ptr<Dto>>* queue) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = std::find(queues.begin(), queues.end(), queue);
    if (it != queues.end()) {
        queues.erase(it);
    }
}

void Broadcaster::broadcast(std::shared_ptr<Dto> dto) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto* queue: queues) {
        queue->try_push(dto);
    }
}
