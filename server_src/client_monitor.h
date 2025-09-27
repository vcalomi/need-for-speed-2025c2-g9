#ifndef CLIENT_MONITOR_H
#define CLIENT_MONITOR_H

#include "../common_src/queue.h"
#include <vector>

class ClientMonitor {
private:
    std::mutex mtx;
    std::vector<Queue<std::vector<uint8_t>>*> clientQueues;

public:
    explicit ClientMonitor();
    
    // Agregar queue de un nuevo cliente
    void addQueue(Queue<std::vector<uint8_t>>* queue);
    
    // Remover queue de un cliente desconectado
    void removeQueue(Queue<std::vector<uint8_t>>* queue);
    
    // Broadcast a todos los clientes conectados
    void broadcast(const std::vector<uint8_t>& message);
};

#endif
