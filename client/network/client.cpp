#include "client.h"

#include <iostream>
#include <sstream>
#include <string>

#include "../core/game.h"

#include "client_input_handler.h"

Client::Client(ClientProtocol& protocol, const std::string& username):
        clientProtocol(protocol),
        username(username),
        connected(true),
        recvQueue(),
        senderQueue(),
        sender(clientProtocol, senderQueue),
        receiver(clientProtocol, recvQueue) {}

void Client::run() {
    try {
        ClientInputHandler input([this]() { this->stop(); });
        input.start();
        sender.start();
        receiver.start();
        std::cout << "Client: Game communication started" << std::endl;
        Game game(*this, username);
        game.Run();
        input.join();
    } catch (const std::exception& e) {
        std::cerr << "Client fatal error: " << e.what() << std::endl;
    }
    stop();
}

void Client::stop() {
    connected = false;
    try {
        clientProtocol.close();
    } catch (...) {}
    try {
        recvQueue.close();
    } catch (...) {}
    try {
        senderQueue.close();
    } catch (...) {}
    sender.stop();
    receiver.stop();
    if (sender.is_alive())
        sender.join();
    if (receiver.is_alive())
        receiver.join();
}

Client::~Client() {}
