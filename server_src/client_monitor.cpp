#include "client_monitor.h"

#include <iostream>
#include <vector>

#include "../common_src/queue.h"

ClientMonitor::ClientMonitor(): clientQueues() {}

void ClientMonitor::addQueue(Queue<std::vector<uint8_t>>* queue) {
    std::lock_guard<std::mutex> lock(mtx);
    clientQueues.push_back(queue);
}

void ClientMonitor::removeQueue(const Queue<std::vector<uint8_t>>* queue) {
    std::lock_guard<std::mutex> lock(mtx);

    for (auto it = clientQueues.begin(); it != clientQueues.end();) {
        if (*it == queue) {
            it = clientQueues.erase(it);
        } else {
            ++it;
        }
    }
}

void ClientMonitor::broadcast(const std::vector<uint8_t>& message) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto* queue: clientQueues) {
        queue->try_push(message);
    }
}
