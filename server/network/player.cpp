#include "player.h"

#include <iostream>

Player::Player(std::shared_ptr<ServerProtocol> protocol, int clientId)
    : protocol(protocol), clientId(clientId), state(State::IN_LOBBY), sendQueue() {}

void Player::beginGame(Queue<std::shared_ptr<Dto>>& gameCommands) {
    if (state == State::IN_GAME) return;
    
    try {
        receiver = std::make_unique<Receiver>(*protocol, gameCommands);
        receiver->start();
        sender = std::make_unique<Sender>(*protocol, sendQueue);
        sender->start();
        state = State::IN_GAME;
    } catch (const std::exception& e) {
        std::cout << "Player " << clientId << ": Error starting game: " << e.what() << std::endl;
        throw;
    }
}

void Player::stopGame() {
    if (state != State::IN_GAME) return;
    
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
    }
    
    if (sender) {
        try {
            sender->stop();
            if (sender->is_alive()) {
                sender->join();
            }
        } catch (...) {}
    }
    
    state = State::IN_LOBBY;
}

Player::~Player() {
    stopGame();
}
