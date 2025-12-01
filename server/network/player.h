#ifndef PLAYER_H
#define PLAYER_H

#include <memory>

#include "../../common/Dto/dto.h"
#include "../../common/queue.h"
#include "server_protocol.h"
#include "receiver.h"
#include "sender.h"

class Player {
public:
    enum class State { IN_LOBBY, IN_GAME };
    
private:
    std::shared_ptr<ServerProtocol> protocol;
    int clientId;
    State state;
    Queue<std::shared_ptr<Dto>> sendQueue;
    std::atomic<bool> is_ready;
    std::unique_ptr<Receiver> receiver;
    std::unique_ptr<Sender> sender;
    
public:
    Player(std::shared_ptr<ServerProtocol> protocol, int clientId);
    
    void beginGame(Queue<std::shared_ptr<Dto>>& gameCommands);
    void stopGame();
    
    bool isGameActive() const { return state == State::IN_GAME; }
    Queue<std::shared_ptr<Dto>>& getSendQueue() { return sendQueue; }
    bool isReady() const { return is_ready; }
    
    ~Player();
};

#endif
