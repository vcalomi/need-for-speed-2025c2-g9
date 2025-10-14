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
            processCommand(command);
        } catch (const std::exception& e) {
            connected = false;
        }
    }
}

void Client::processCommand(const std::string& command) {
    if (command == "exit") {
        connected = false;
    } else if (command == "nitro") {
        clientProtocol.sendNitro();
    } else if (command.compare(0, 5, "read ") == 0) {
        std::stringstream ss(command);
        std::string cmd;
        int count = 0;
        ss >> cmd;

        if (ss >> count) {
            if (count > 0) {
                readMessages(count);
            }
        }
    }
}

void Client::readMessages(int count) {
    try {
        for (int i = 0; i < count && connected; i++) {
            NitroMessage message = clientProtocol.receiveMessage();

            uint8_t eventType = uint8_t(message.nitroState);
            if (eventType == uint8_t(ActionCode::NITRO_ACTIVATED)) {
                std::cout << "A car hit the nitro!" << std::endl;
            } else if (eventType == uint8_t(ActionCode::NITRO_DEACTIVATED)) {
                std::cout << "A car is out of juice." << std::endl;
            }
        }

    } catch (const std::exception& e) {
        connected = false;
    }
}

Client::~Client() {}
