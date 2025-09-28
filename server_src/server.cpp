#include "server.h"
#include "../common_src/queue.h"

Server::Server(const std::string& port) :
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
    } catch (const std::exception& e) { stop(); }
    return 0;
}

void Server::stop() {
    clientAcceptor.close();
    gameLoopQueue.close();
    gameLoop.stop();
    clientAcceptor.join();
    gameLoop.join();
    clientAcceptor.clear();
}

Server::~Server() {}
