#include "player.h"

#include <iostream>

Player::Player(std::shared_ptr<ServerProtocol> protocol, int clientId)
    : protocol(protocol), clientId(clientId), state(State::IN_LOBBY), sendQueue(), is_ready(false) {}

void Player::beginGame(Queue<std::shared_ptr<Dto>>& gameCommands) {
    if (state == State::IN_GAME) return;
    
    try {
        receiver = std::make_unique<Receiver>(*protocol, gameCommands);
        receiver->start();
        sender = std::make_unique<Sender>(*protocol, sendQueue);
        sender->start();
        state = State::IN_GAME;
        is_ready = true;
    } catch (const std::exception& e) {
        std::cout << "Player " << clientId << ": Error starting game: " << e.what() << std::endl;
        throw;
    }
}

void Player::stopGame() {
    if (state != State::IN_GAME) return;
    is_ready = false;
    try { 
        sendQueue.close(); 
    } catch (...) {}
    
    if (sender) {
        try {
            sender->stop();
            try {
                sender->join();
            } catch (...) {}
            sender.reset();
        } catch (...) {}
    }

    if (protocol) {
        protocol.reset();
    }
    
    if (receiver) {
        try {
            receiver->stop();
            try {
                receiver->join();
            } catch (...) {}
            receiver.reset();
        } catch (...) {}
    }
    
    state = State::IN_LOBBY;
}

Player::~Player() {
    stopGame();
}
