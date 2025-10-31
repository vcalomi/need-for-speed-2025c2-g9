#ifndef RECEIVER_H
#define RECEIVER_H

#include "../common_src/client_command.h"
#include "../common_src/queue.h"
#include "../common_src/socket.h"
#include "../common_src/socket_closed.h"
#include "../common_src/thread.h"
#include "../common_src/Dto/dto.h"

#include "server_protocol.h"

#include <optional>

class Receiver: public Thread {
private:
     Queue<std::shared_ptr<Dto>>& gameQueue;
    ServerProtocol& protocol;

public:
    Receiver(ServerProtocol& serverProtocol,  Queue<std::shared_ptr<Dto>>& gameQueue);
    void run() override;
    ~Receiver();
};

#endif
