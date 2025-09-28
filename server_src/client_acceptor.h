#ifndef CLIENT_ACCEPTOR_H
#define CLIENT_ACCEPTOR_H

#include <vector>
#include "../common_src/socket.h"
#include "../common_src/thread.h"
#include "client_handler.h"
#include "client_monitor.h"
#include "../common_src/liberror.h"

class ClientAcceptor : public Thread {
private:
    ClientMonitor& clientMonitor;
    Queue<ActionCode>& gameLoopQueue;
    Socket acceptor;
    // std::atomic<bool> keep_running;
    std::vector<ClientHandler*> clients;

public:
    ClientAcceptor(const std::string& port,  ClientMonitor& monitor, Queue<ActionCode>& queue);
    void run() override;
    void close();
    void reap();
    void clear();
    ~ClientAcceptor();
};

#endif