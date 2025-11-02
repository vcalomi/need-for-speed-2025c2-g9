#include "server.h"

#include <string>

#include "../common/queue.h"

Server::Server(const std::string& port): gameLobby(), acceptor(port, gameLobby), inputHandler() {}

int Server::run() {
    try {
        inputHandler.start();
        acceptor.start();

        inputHandler.join();
        stop();
    } catch (const std::exception& e) {
        stop();
    }
    return 0;
}

void Server::stop() {
    acceptor.close();
    acceptor.join();
}

Server::~Server() {}
