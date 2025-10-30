#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include "../common_src/queue.h"
#include "../common_src/socket.h"
#include "../common_src/socket_closed.h"
#include "../common_src/thread.h"
#include "../common_src/Dto/dto.h"

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
