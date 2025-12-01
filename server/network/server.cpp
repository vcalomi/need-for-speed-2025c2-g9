#include "server.h"

#include <string>

#include "../../common/queue.h"

Server::Server(const std::string& port):
        gameMonitor(), acceptor(port, gameMonitor), inputHandler() {

    inputHandler.setShutdownCallback([this]() {
        this->stop();
    });
}

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
    try {
        acceptor.close();
    } catch (...) {}
    try {
        acceptor.join();
    } catch (...) {}
    
    try {
        gameMonitor.closeAll();
    } catch (...) {}
}

Server::~Server() {}
