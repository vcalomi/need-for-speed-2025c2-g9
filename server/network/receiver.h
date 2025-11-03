#include <memory>
#ifndef RECEIVER_H
#define RECEIVER_H

#include <optional>

#include "../../common/Dto/dto.h"
#include "../../common/client_command.h"
#include "../../common/queue.h"
#include "../../common/socket.h"
#include "../../common/socket_closed.h"
#include "../../common/thread.h"

#include "server_protocol.h"

class Receiver: public Thread {
private:
    Queue<std::shared_ptr<Dto>>& gameQueue;
    ServerProtocol& protocol;

public:
    Receiver(ServerProtocol& serverProtocol, Queue<std::shared_ptr<Dto>>& gameQueue);
    void run() override;
    ~Receiver();
};

#endif
