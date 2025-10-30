#include "broadcaster.h"
#include <algorithm>

void Broadcaster::addQueue(Queue<Dto>* queue) {
    std::lock_guard<std::mutex> lock(mtx);
    queues.push_back(queue);
}

void Broadcaster::removeQueue(Queue<Dto>* queue) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = std::find(queues.begin(), queues.end(), queue);
    if (it != queues.end()) {
        queues.erase(it);
    }
}

void Broadcaster::broadcast(const Dto& message) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto* queue : queues) {
        queue->try_push(message);
    }
}