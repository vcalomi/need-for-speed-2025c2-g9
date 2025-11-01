#ifndef LOBBY_WORKER_H
#define LOBBY_WORKER_H

#include <atomic>
#include <functional>
#include <memory>
#include <thread>

#include "server_protocol.h"
#include "game_lobby.h"
#include "command_dispatcher.h"
#include "../common/Dto/dto.h"
#include "../common/queue.h"

class LobbyWorker {
public:
    LobbyWorker(ServerProtocol& protocol,
                GameLobby& lobby,
                int clientId,
                Queue<std::shared_ptr<Dto>>& senderQueue,
                std::function<void()> onStartGame);

    void start();
    void stop();
    void join();
    bool is_alive() const;

private:
    void run();
    void initHandlers();

    // Handlers
    void handleListRooms();
    void handleCreateRoom();
    void handleJoinRoom();
    void handleStartGame();
    void handleChooseCar();
    void handleSendUsername();

    ServerProtocol& protocol;
    GameLobby& lobby;
    int clientId;
    Queue<std::shared_ptr<Dto>>& senderQueue;
    std::function<void()> onStartGame;

    std::thread th;
    std::atomic_bool keep_running{true};
    std::atomic_bool alive{false};
    CommandDispatcher dispatcher;
};

#endif // LOBBY_WORKER_H
