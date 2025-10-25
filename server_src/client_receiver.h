#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include "../common_src/client_command.h"
#include "../common_src/queue.h"
#include "../common_src/socket.h"
#include "../common_src/socket_closed.h"
#include "../common_src/thread.h"

#include "server_protocol.h"
#include "game_lobby.h"
#include "../common_src/nitro_message.h"

#include <optional>

class ClientReceiver: public Thread {
private:
    Queue<ClientCommand>& gameQueue;
    ServerProtocol& protocol;
    int clientId;

public:
    ClientReceiver(ServerProtocol& serverProtocol, Queue<ClientCommand>& gameQueue,
                   int clientId);
    void run() override;
    ~ClientReceiver();
};

#endif
