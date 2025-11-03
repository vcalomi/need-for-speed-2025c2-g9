#ifndef LOBBY_H
#define LOBBY_H

#include <atomic>
#include <functional>
#include <memory>
#include <thread>

#include "../../common/Dto/dto.h"
#include "../../common/queue.h"
#include "../../common/thread.h"

#include "command_dispatcher.h"
#include "game_monitor.h"
#include "server_protocol.h"

class Lobby: public Thread {
private:
    ServerProtocol& protocol;
    GameMonitor& gameMonitor;
    int clientId;
    Queue<std::shared_ptr<Dto>>& senderQueue;
    std::function<void(std::shared_ptr<GameRoom> room)> onStartGame;
    CommandDispatcher dispatcher;

    void initHandlers();
    // Handlers
    void handleListRooms();
    void handleCreateRoom();
    void handleJoinRoom();
    void handleStartGame();
    void handleListPlayers();
    void handleListState();
    void handleChooseCar();
    void handleSendUsername();


public:
    Lobby(ServerProtocol& protocol, GameMonitor& gameMonitor, int clientId,
          Queue<std::shared_ptr<Dto>>& senderQueue,
          std::function<void(std::shared_ptr<GameRoom> room)> onStartGame);
    void run() override;
};

#endif
