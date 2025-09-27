#include "server.h"
#include "../common_src/queue.h"

Server::Server(const std::string& port) :
     gameLoopQueue(100), 
     clientMonitor(), 
     clientAcceptor(port, clientMonitor, gameLoopQueue), 
     gameLoop(gameLoopQueue, clientMonitor),  
     running(true) {}

void Server::run() {
    // running = true;

    clientAcceptor.start(); // inicio el thread
    gameLoop.start();
    while(running) {
        try {
            // clientAcceptor.start(); // inicio el thread
            if (std::cin.get() == 'q') { 
                running = false;
                break; 
            }
            // clientAcceptor.join();
        } catch(const std::exception& e) {
            continue;
        }
    }
    stopServer();
}

void Server::stopServer() {
    running = false;
    
    // Detener los threads
    clientAcceptor.stop();
    gameLoop.stop();
    
    // Esperar a que terminen
    clientAcceptor.join();
    gameLoop.join();
}

Server::~Server() {}
