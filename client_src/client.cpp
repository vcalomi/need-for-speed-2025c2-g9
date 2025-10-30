#include "client.h"
#include <iostream>
#include <sstream>
#include <string>

Client::Client(const std::string& hostname, const std::string& port):
        clientProtocol(hostname, port), connected(true), recvQueue(), receiver(nullptr) {}

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
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;
    if (cmd.empty()) return;

    if (cmd == "LIST_ROOMS") {
        clientProtocol.sendListRooms();
        try {
            auto rooms = clientProtocol.receiveRoomList();
            for (const auto& r : rooms) {
                std::cout << r << std::endl;
            }
        } catch (const std::exception&) {
        }
        return;
    }
    if (cmd == "CREATE_ROOM" || cmd == "CREATE_GAME") {
        std::string roomName;
        std::getline(iss, roomName);
        if (!roomName.empty() && roomName.front() == ' ') roomName.erase(0, 1);
        if (!roomName.empty()) clientProtocol.sendCreateRoom(roomName);
        clientProtocol.receiveAction();
        return;
    }
    if (cmd == "JOIN_ROOM") {
        std::string roomName;
        std::getline(iss, roomName);
        if (!roomName.empty() && roomName.front() == ' ') roomName.erase(0, 1);
        if (!roomName.empty()) clientProtocol.sendJoinRoom(roomName);
        return;
    }
    if (cmd == "START_GAME") {
        clientProtocol.sendStartGame();
        if (!receiver) {
            receiver = std::make_unique<ClientReceiver>(clientProtocol, recvQueue);
            receiver->start();
        }
        return;
    }
    if (cmd == "CHOOSE_CAR") {
        std::string carType;
        iss >> carType;
        if (!carType.empty()) clientProtocol.sendChooseCar(carType);
        return;
    }
    if (cmd == "EXIT") {
        connected = false;
        clientProtocol.close();
        if (receiver) {
            receiver->stop();
            receiver->join();
        }
        return;
    }
}

Client::~Client() {}