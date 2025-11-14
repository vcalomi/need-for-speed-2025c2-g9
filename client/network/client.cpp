#include "client.h"

#include <iostream>
#include <sstream>
#include <string>

#include "../app/game.h"

Client::Client(ClientProtocol& protocol, const std::string& username):
        clientProtocol(protocol),
        username(username),
        connected(true),
        recvQueue(),
        senderQueue(),
        sender(clientProtocol, senderQueue),
        receiver(clientProtocol, recvQueue) {}

void Client::run() {
    sender.start();
    receiver.start();
    std::cout << "Client: Game communication started" << std::endl;
    
    try {
        Game game(*this);
        game.Run();
    } catch (const std::exception& e) {
        std::cerr << "Client fatal error: " << e.what() << std::endl;
    }
    stop();
}

void Client::stop() {
    if (!connected) {
        return;
    }
    connected = false;
    try {
        recvQueue.close();
    } catch (...) {}
    try {
        senderQueue.close();
    } catch (...) {}
    
    sender.stop();
    receiver.stop();
    
    try {
        clientProtocol.close();
    } catch (...) {}

    if (sender.is_alive())
        sender.join();

    if (receiver.is_alive())
        receiver.join();
}

Client::~Client() {}
