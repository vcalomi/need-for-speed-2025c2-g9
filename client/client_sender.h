#include <memory>
#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include <vector>

#include "../common/Dto/dto.h"
#include "../common/queue.h"
#include "../common/socket.h"
#include "../common/socket_closed.h"
#include "../common/thread.h"

#include "client_protocol.h"

class ClientSender: public Thread {
private:
    Queue<std::shared_ptr<Dto>>& clientQueue;
    ClientProtocol& protocol;

public:
    explicit ClientSender(ClientProtocol& clientProtocol, Queue<std::shared_ptr<Dto>>& clientQueue);
    void run() override;
    ~ClientSender();
};

#endif
