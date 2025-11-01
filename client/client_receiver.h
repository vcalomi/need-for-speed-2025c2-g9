#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include "../common/queue.h"
#include "../common/socket.h"
#include "../common/socket_closed.h"
#include "../common/thread.h"
#include "../common/Dto/dto.h"

#include "client_protocol.h"

#include <optional>

class ClientReceiver: public Thread {
private:
    Queue<std::shared_ptr<Dto>>& queue;
    ClientProtocol& protocol;

public:
    ClientReceiver(ClientProtocol& clientProtocol, Queue<std::shared_ptr<Dto>>& queue);
    void run() override;
    ~ClientReceiver();
};

#endif
