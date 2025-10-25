#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include <vector>

#include "../common_src/nitro_message.h"
#include "../common_src/queue.h"
#include "../common_src/socket.h"
#include "../common_src/socket_closed.h"
#include "../common_src/thread.h"

#include "server_protocol.h"

class ClientSender: public Thread {
private:
    Queue<NitroMessage>& clientQueue;
    ServerProtocol& protocol;

public:
    explicit ClientSender(ServerProtocol& serverProtocol, Queue<NitroMessage>& clientQueue);
    void run() override;
    ~ClientSender();
};

#endif
