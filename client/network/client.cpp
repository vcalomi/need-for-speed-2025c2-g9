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
    
    try {
        Game game(*this);
        game.Run();
    } catch (const std::exception& e) {}
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
    
    if (sender.is_alive()) {
        sender.join();
    }
    
    if (receiver.is_alive()) {
        receiver.join();
    }

    try {
        clientProtocol.close();
    } catch (...) {}
}

Client::~Client() {}
