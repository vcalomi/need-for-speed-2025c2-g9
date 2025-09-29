#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include "../common_src/client_command.h"
#include "../common_src/queue.h"
#include "../common_src/socket.h"
#include "../common_src/socket_closed.h"
#include "../common_src/thread.h"

#include "server_protocol.h"

class ClientReceiver: public Thread {
private:
    Queue<ClientCommand>& gameLoopQueue;
    ServerProtocol& protocol;
    int clientId;

public:
    ClientReceiver(ServerProtocol& serverProtocol, Queue<ClientCommand>& gameLoopQueue,
                   int clientId);
    void run() override;
    ~ClientReceiver();
};

#endif
