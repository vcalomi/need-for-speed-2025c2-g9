#include "client.h"

#include <iostream>
// #include <QApplication>
#include <sstream>
#include <string>

Client::Client(const std::string& hostname, const std::string& port):
        clientProtocol(hostname, port),
        connected(true),
        recvQueue(),
        senderQueue(),
        sender(clientProtocol, senderQueue),
        receiver(clientProtocol, recvQueue) {}

void Client::run() {
    while (connected) {
        try {
            sender.start();
            receiver.start();
            std::cout << "Client: Game communication started" << std::endl;
        } catch (const std::exception& e) {
            connected = false;
        }
    }
    stop();
}

void Client::stop() {
    connected = false;
    clientProtocol.close();
    recvQueue.close();
    senderQueue.close();
    sender.stop();
    sender.join();
    receiver.stop();
    receiver.join();
}

Client::~Client() {}
