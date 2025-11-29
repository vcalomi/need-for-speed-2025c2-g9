#include "player.h"

#include <iostream>

#include "receiver.h"
#include "sender.h"

Player::Player(std::shared_ptr<ServerProtocol> protocol, int clientId):
        protocol(protocol),
        clientId(clientId),
        in_game(false),
        sendQueue() {}

void Player::startGame(Queue<std::shared_ptr<Dto>>& gameCommands) {
    if (in_game) {
        return;
    }
    
    try {
        receiver = std::make_unique<Receiver>(*protocol, gameCommands);
        receiver->start();

        sender = std::make_unique<Sender>(*protocol, sendQueue);
        sender->start();
        
        in_game = true;
        
    } catch (const std::exception& e) {
        std::cout << "Player " << clientId << ": Error starting game: " << e.what() << std::endl;
    }
}

void Player::stopGame() {
    if (!in_game) {
        return;
    }

    try {
        sendQueue.close();
    } catch (...) {}

    if (receiver) {
        try {
            receiver->stop();
            if (receiver->is_alive()) {
                receiver->join();
            }
        } catch (...) {}
        receiver.reset();
    }

    if (sender) {
        try {
            sender->stop();
            if (sender->is_alive()) {
                sender->join();
            }
        } catch (...) {}
        sender.reset();
    }
    
    in_game = false;
}

Player::~Player() {
    stopGame();
}
