#include "server.h"

#include <string>

#include "../common_src/queue.h"

Server::Server(const std::string& port):
        gameLoopQueue(100),
        clientMonitor(),
        clientAcceptor(port, clientMonitor, gameLoopQueue),
        gameLoop(gameLoopQueue, clientMonitor),
        inputHandler() {}

int Server::run() {
    try {
        inputHandler.start();
        clientAcceptor.start();
        gameLoop.start();

        inputHandler.join();
        stop();
    } catch (const std::exception& e) {
        stop();
    }
    return 0;
}

void Server::stop() {
    clientAcceptor.close();
    clientAcceptor.join();
    gameLoopQueue.close();
    gameLoop.stop();
    gameLoop.join();
}

Server::~Server() {}
