#ifndef SERVER_H
#define SERVER_H
#include <string>

#include "client_acceptor.h"
#include "client_monitor.h"
#include "gameloop.h"
#include "../common_src/queue.h"

class Server {
private:
    Queue<ActionCode> gameLoopQueue;
    ClientMonitor clientMonitor;
    ClientAcceptor clientAcceptor;
    GameLoop gameLoop;
    bool running;

public:
    Server(const std::string& port);
    void run();
    void stopServer();
    ~Server();
};

#endif
