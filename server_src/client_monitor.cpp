#include "client_monitor.h"

#include <iostream>
#include <vector>

#include "../common_src/queue.h"

ClientMonitor::ClientMonitor(): clientQueues() {}

void ClientMonitor::addQueue(Queue<NitroMessage>* queue) {
    std::lock_guard<std::mutex> lock(mtx);
    clientQueues.push_back(queue);
}

void ClientMonitor::removeQueue(const Queue<NitroMessage>* queue) {
    std::lock_guard<std::mutex> lock(mtx);

    for (auto it = clientQueues.begin(); it != clientQueues.end();) {
        if (*it == queue) {
            it = clientQueues.erase(it);
        } else {
            ++it;
        }
    }
}

void ClientMonitor::broadcast(const NitroMessage& message) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto* queue: clientQueues) {
        queue->try_push(message);
    }
}
