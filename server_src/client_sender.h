#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include "../common_src/thread.h"
#include "../common_src/socket.h"
#include "server_protocol.h"
#include "../common_src/queue.h"

class ClientSender : public Thread {
private:
    Socket &peer;
    Queue<std::vector<uint8_t>> clientQueue;
    ServerProtocol protocol;
    std::atomic<bool> keep_running;
public:
    explicit ClientSender(Socket& socket);
    void run() override;
    void stop() override;
    ~ClientSender();
};

#endif