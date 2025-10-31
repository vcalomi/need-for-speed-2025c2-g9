#ifndef SENDER_H
#define SENDER_H

#include <vector>

#include "../common_src/queue.h"
#include "../common_src/socket.h"
#include "../common_src/socket_closed.h"
#include "../common_src/thread.h"

#include "client_protocol.h"

class Sender: public Thread {
private:
    Queue<std::shared_ptr<Dto>>& clientQueue;
    ClientProtocol& protocol;

public:
    explicit Sender(ClientProtocol& clientProtocol, Queue<std::shared_ptr<Dto>>& clientQueue);
    void run() override;
    ~Sender();
};

#endif
