#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include "../common_src/thread.h"
#include "../common_src/socket.h"
#include "server_protocol.h"
#include "../common_src/queue.h"
#include "../common_src/socket_closed.h"

class ClientSender : public Thread {
private:
    Queue<std::vector<uint8_t>>& clientQueue;
    ServerProtocol& protocol;
    std::atomic<bool> keep_running;
public:
    explicit ClientSender(ServerProtocol& serverProtocol, Queue<std::vector<uint8_t>>& clientQueue);
    void run() override;
    ~ClientSender();
};

#endif