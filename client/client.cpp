#include "client.h"
#include <iostream>
// #include <QApplication>
#include <sstream>
#include <string>

Client::Client(const std::string& hostname, const std::string& port):
        clientProtocol(hostname, port), connected(true), recvQueue(), receiver(nullptr) {}

void Client::run() {
    while (connected) {
        try {
            // QApplication a(argc, argv);
        } catch (const std::exception& e) {
            connected = false;
        }
    }
}

void Client::stop() {
    connected = false;
    clientProtocol.close();
    recvQueue.close();
    // senderQueeue.close();
    receiver->stop();
    receiver->join();
    // sender.stop();
}

Client::~Client() {}