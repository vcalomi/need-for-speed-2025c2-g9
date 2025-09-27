#include "client.h"
#include <iostream>
#include <sstream>
#include <string>

Client::Client(const std::string& hostname, const std::string& port) :
    clientProtocol(hostname, port), connected(true) {}

void Client::run() {
    while (connected) {
        try {
            std::string command;
            std::cout << "> ";
            std::getline(std::cin, command);
            
            processCommand(command);
            
        } catch (const std::exception& e) {
        }
    }
}

void Client::processCommand(const std::string& command) {
    if (command == "exit") {
        connected = false;
    }
    else if (command == "nitro") {
        clientProtocol.sendNitro();
    }
    else if (command.find("read ") == 0) {
        std::stringstream ss(command);
        std::string cmd;
        int count = 0;
        ss >> cmd;
        
        if (ss >> count) {
            if (count > 0) {
                readMessages(count);
            } else {
                std::cout << "Número inválido: " << count << "\n";
            }
        }
    }
}

void Client::readMessages(int count) {
    try {
        for (int i = 0; i < count && connected; i++) {
            std::vector<uint8_t> message = clientProtocol.receiveMessage();
            
            uint8_t eventType = message[3];
            if (eventType == uint8_t(ActionCode::NITRO_ACTIVATED)) {
                std::cout << "A car hit the nitro!\n";
            } else if (eventType == uint8_t(ActionCode::NITRO_DEACTIVATED)) {
                std::cout << "A car is out of juice.\n";
            }
        }
        
    } catch (const std::exception& e) {
        std::cout << "Error recibiendo mensajes: " << e.what() << "\n";
        connected = false;
    }
}

Client::~Client() {}
