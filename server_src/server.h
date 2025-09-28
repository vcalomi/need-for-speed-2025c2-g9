#ifndef SERVER_H
#define SERVER_H
#include <string>

#include "client_acceptor.h"
#include "client_monitor.h"
#include "gameloop.h"
#include "../common_src/queue.h"
#include "server_input_handler.h"
#include "../common_src/client_command.h"

class Server {
private:
    Queue<ClientCommand> gameLoopQueue;
    ClientMonitor clientMonitor;
    ClientAcceptor clientAcceptor;
    GameLoop gameLoop;
    ServerInputHandler inputHandler;

public:
    Server(const std::string& port);
    int run();
    void stop();
    ~Server();
};

#endif
