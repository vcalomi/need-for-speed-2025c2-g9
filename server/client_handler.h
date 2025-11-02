#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <vector>
#include "../common/socket.h"
#include "../common/socket_closed.h"
#include "../common/thread.h"
#include "../common/queue.h"

#include "client_handler.h"
#include "game_monitor.h"
#include "receiver.h"
#include "sender.h"
#include "server_protocol.h"
#include "client_state.h"
#include "../common/Dto/dto.h"
#include "lobby.h"

class ClientHandler {
private:
    Socket peer;
    GameMonitor& gameMonitor;
    ServerProtocol protocol;
    std::atomic_bool keep_running;
    Queue<std::shared_ptr<Dto>> senderQueue;    // para enviar al cliente
    ClientState state;
    int clientId;
    bool gameStarted;
    Lobby lobby;
    std::unique_ptr<Receiver> receiver;
    std::unique_ptr<Sender> sender;

public:
    ClientHandler(Socket socket, GameMonitor& gameMonitor, int clientId);
    void startThreads(std::shared_ptr<GameRoom> room);
    void start();
    void join();
    void stop();
    bool is_alive() const;
    ~ClientHandler();
};

#endif
