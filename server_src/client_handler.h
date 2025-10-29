#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <vector>

#include "../common_src/client_command.h"
#include "../common_src/socket.h"
#include "../common_src/socket_closed.h"
#include "../common_src/thread.h"
#include "../common_src/nitro_message.h"
#include "../common_src/queue.h"

#include "client_handler.h"
#include "game_lobby.h"
#include "receiver.h"
#include "sender.h"
#include "server_protocol.h"
#include "client_state.h"

class ClientHandler {
private:
    Socket peer;
    GameLobby& gameLobby;
    ServerProtocol protocol;
    std::atomic_bool keep_running;
    ClientState state;
    int clientId;
    Queue<NitroMessage> senderQueue;    // para enviar al cliente
    std::unique_ptr<Receiver> receiver;;
    Sender sender;

public:
    ClientHandler(Socket socket, GameLobby& gameLobby, int clientId);
    void start();
    void join();
    void stop();
    bool is_alive() const;
    void startGameThreads(Queue<ClientCommand>& gameQueue);
    void handleLobbyCommand(ActionCode action);
    ~ClientHandler();
};

#endif
