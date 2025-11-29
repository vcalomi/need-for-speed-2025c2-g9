#ifndef PLAYER_H
#define PLAYER_H

#include <atomic>
#include <memory>

#include "../../common/queue.h"
#include "../../common/socket.h"
#include "../../common/thread.h"

#include "receiver.h"
#include "sender.h"
#include "server_protocol.h"

class Player {
private:
    std::shared_ptr<ServerProtocol> protocol;
    int clientId;
    std::atomic<bool> in_game;
    std::unique_ptr<Receiver> receiver;
    std::unique_ptr<Sender> sender;
    Queue<std::shared_ptr<Dto>> sendQueue;

public:
    Player(std::shared_ptr<ServerProtocol> protocol, int clientId);
    void startGame(Queue<std::shared_ptr<Dto>>& gameCommands);
    void stopGame();
    Queue<std::shared_ptr<Dto>>& getSendQueue() { return sendQueue; }
    bool isInGame() const { return in_game; }
    
    ~Player();
};

#endif
