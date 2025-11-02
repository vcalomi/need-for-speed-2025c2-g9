#ifndef SERVER_H
#define SERVER_H
#include <string>

#include "../common/client_command.h"
#include "../common/queue.h"

#include "acceptor.h"
#include "server_input_handler.h"
#include "game_monitor.h"

class Server {
private:
    GameMonitor gameMonitor;
    Acceptor acceptor;
    ServerInputHandler inputHandler;

public:
    explicit Server(const std::string& port);
    int run();
    void stop();
    ~Server();
};

#endif
