#include "client.h"
#include <iostream>
#include <sstream>
#include <string>

Client::Client(const std::string& hostname, const std::string& port):
        clientProtocol(hostname, port), connected(true) {}

void Client::run() {
    while (connected) {
        try {
            std::string command;
            if (!std::getline(std::cin, command)) {
                connected = false;
                break;
            }
            // processCommand(command);
        } catch (const std::exception& e) {
            connected = false;
        }
    }
}

Client::~Client() {}