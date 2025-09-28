#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include "../common_src/thread.h"
#include "../common_src/socket.h"
#include "server_protocol.h"
#include "../common_src/queue.h"
#include "../common_src/socket_closed.h"
#include "../common_src/client_command.h"

class ClientReceiver : public Thread {
private:
    Queue<ClientCommand>& gameLoopQueue;
    ServerProtocol& protocol;
    std::atomic_bool keep_running;
    int clientId;

public:
    ClientReceiver(ServerProtocol& serverProtocol, Queue<ClientCommand>& gameLoopQueue, int clientId);
    void run() override;
    ~ClientReceiver();
};

#endif
