#ifndef LOBBY_WORKER_H
#define LOBBY_WORKER_H

#include <atomic>
#include <functional>
#include <memory>
#include <thread>

#include "../common/Dto/dto.h"
#include "../common/queue.h"
#include "../common/thread.h"

#include "command_dispatcher.h"
#include "game_lobby.h"
#include "server_protocol.h"

class LobbyWorker: public Thread {
private:
    ServerProtocol& protocol;
    GameLobby& lobby;
    int clientId;
    Queue<std::shared_ptr<Dto>>& senderQueue;
    std::function<void()> onStartGame;
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
    LobbyWorker(ServerProtocol& protocol, GameLobby& lobby, int clientId,
                Queue<std::shared_ptr<Dto>>& senderQueue, std::function<void()> onStartGame);
    void run() override;
};

#endif
