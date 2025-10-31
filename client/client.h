#ifndef CLIENT_H
#define CLIENT_H
#include <string>
#include <vector>

#include "client_protocol.h"
#include "../common_src/queue.h"
#include "client_receiver.h"
#include "../common_src/Dto/dto.h"

class Client {
private:
    ClientProtocol clientProtocol;
    bool connected;
    Queue<std::shared_ptr<Dto>> recvQueue;
    std::unique_ptr<ClientReceiver> receiver;

public:
    Client(const std::string& hostname, const std::string& port);
    void run();
    void stop();
    ~Client();
};

#endif
