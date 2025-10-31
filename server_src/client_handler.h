#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <vector>
#include "../common_src/socket.h"
#include "../common_src/socket_closed.h"
#include "../common_src/thread.h"
#include "../common_src/queue.h"

#include "client_handler.h"
#include "game_lobby.h"
#include "receiver.h"
#include "sender.h"
#include "server_protocol.h"
#include "client_state.h"
#include "../common_src/Dto/dto.h"

class ClientHandler {
private:
    Socket peer;
    GameLobby& gameLobby;
    ServerProtocol protocol;
    std::atomic_bool keep_running;
    Queue<std::shared_ptr<Dto>> senderQueue;    // para enviar al cliente
    Sender sender;
    ClientState state;
    int clientId;
    std::unique_ptr<Receiver> receiver;

public:
    ClientHandler(Socket socket, GameLobby& lobby, int clientId);
    void start();
    void join();
    void stop();
    bool is_alive() const;
    void handleLobbyCommand(ActionCode action);
    ~ClientHandler();
};

#endif
