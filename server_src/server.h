#ifndef SERVER_H
#define SERVER_H
#include <string>

#include "../common_src/client_command.h"
#include "../common_src/queue.h"

#include "acceptor.h"
#include "server_input_handler.h"

class Server {
private:
    Acceptor acceptor;
    ServerInputHandler inputHandler;

public:
    explicit Server(const std::string& port);
    int run();
    void stop();
    ~Server();
};

#endif
