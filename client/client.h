#ifndef CLIENT_H
#define CLIENT_H
#include <string>
#include <vector>

#include "client_protocol.h"
#include "../common/queue.h"
#include "client_receiver.h"
#include "../common/Dto/dto.h"
#include "client_receiver.h"
#include "client_sender.h"

class Client {
private:
    ClientProtocol& clientProtocol;
    bool connected;
    Queue<std::shared_ptr<Dto>> recvQueue;
    Queue<std::shared_ptr<Dto>> senderQueue;
    ClientSender sender;
    ClientReceiver receiver;

public:
    Client(ClientProtocol& protocol);
    void run();
    void stop();
    ~Client();
};

#endif
