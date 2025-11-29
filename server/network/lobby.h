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
#include "player_info.h"
#include "server_protocol.h"

class Lobby: public Thread {
private:
    Socket peer;
    GameMonitor& gameMonitor;
    int clientId;
    std::atomic<bool> game_started;
    std::shared_ptr<ServerProtocol> protocol;
    CommandDispatcher dispatcher;

    void initHandlers();
    // Handlers
    void handleListRooms(std::shared_ptr<Dto> dto);
    void handleCreateRoom(std::shared_ptr<Dto> dto);
    void handleJoinRoom(std::shared_ptr<Dto> dto);
    void handleStartGame(std::shared_ptr<Dto> dto);
    void handleListPlayers(std::shared_ptr<Dto> dto);
    void handleListState(std::shared_ptr<Dto> dto);
    void handleChooseCar(std::shared_ptr<Dto> dto);
    void handleSendUsername(std::shared_ptr<Dto> dto);
    void handleSelectMaps(std::shared_ptr<Dto> dto);

public:
    Lobby(Socket socket, GameMonitor& gameMonitor, int clientId);
    void run() override;
    bool isGameStarted() { return game_started; }
};

#endif
