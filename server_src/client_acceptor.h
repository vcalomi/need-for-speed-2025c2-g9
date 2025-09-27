#ifndef CLIENT_ACCEPTOR_H
#define CLIENT_ACCEPTOR_H

#include <vector>
#include "../common_src/socket.h"
#include "../common_src/thread.h"
#include "client_handler.h"
#include "client_monitor.h"

class ClientAcceptor : public Thread {
private:
    ClientMonitor& clientMonitor;
    Queue<ActionCode>& gameLoopQueue;
    Socket acceptor;
    std::vector<ClientHandler*> clients;

public:
    ClientAcceptor(const std::string& port,  ClientMonitor& monitor, Queue<ActionCode>& queue);
    // void reap();
    void run() override;
    void stop() override;
    void cleanupDisconnectedClients();
    void clear();
    ~ClientAcceptor();
};

#endif