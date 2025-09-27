#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include "../common_src/thread.h"
#include "../common_src/socket.h"
#include "server_protocol.h"
#include "../common_src/queue.h"

class ClientReceiver : public Thread {
private:
    Socket& peer;
    Queue<ActionCode>& gameLoopQueue;
    ServerProtocol protocol;
    std::atomic<bool> keep_running;

public:
    ClientReceiver(Socket& socket, Queue<ActionCode>& gameLoopQueue);
    void run();
    ~ClientReceiver();
};

#endif
