#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include "../common_src/queue.h"
#include "../common_src/socket.h"
#include "../common_src/socket_closed.h"
#include "../common_src/thread.h"

#include "client_protocol.h"

#include <optional>

class ClientReceiver: public Thread {
private:
    Queue<>& queue;
    ClientProtocol& protocol;

public:
    ClientReceiver(ClientProtocol& clientProtocol, Queue<>& queue);
    void run() override;
    ~ClientReceiver();
};

#endif
