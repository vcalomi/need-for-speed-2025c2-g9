#include <memory>
#ifndef SENDER_H
#define SENDER_H

#include <vector>

#include "../common/Dto/dto.h"
#include "../common/queue.h"
#include "../common/socket.h"
#include "../common/socket_closed.h"
#include "../common/thread.h"

#include "server_protocol.h"

class Sender: public Thread {
private:
    Queue<std::shared_ptr<Dto>>& clientQueue;
    ServerProtocol& protocol;

public:
    explicit Sender(ServerProtocol& serverProtocol, Queue<std::shared_ptr<Dto>>& clientQueue);
    void run() override;
    ~Sender();
};

#endif
