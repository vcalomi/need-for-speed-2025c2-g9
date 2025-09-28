#include "client_monitor.h"
#include "../common_src/queue.h"
#include <iostream>
#include <vector>

ClientMonitor::ClientMonitor() : clientQueues() {}

void ClientMonitor::addQueue(Queue<std::vector<uint8_t>>* queue) {
    std::lock_guard<std::mutex> lock(mtx);
    clientQueues.push_back(queue);
}

void ClientMonitor::removeQueue(Queue<std::vector<uint8_t>>* queue) {
    std::lock_guard<std::mutex> lock(mtx);
    
    for (auto it = clientQueues.begin(); it != clientQueues.end(); ) {
        if (*it == queue) {
            it = clientQueues.erase(it);
        } else {
            ++it;
        }
    }
}

void ClientMonitor::broadcast(const std::vector<uint8_t>& message) {
    std::lock_guard<std::mutex> lock(mtx);
    
    for (auto* queue : clientQueues) {
        try {
            queue->push(message);
        } catch (const std::exception& e) {
            continue;
        }
    }
}