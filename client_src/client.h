#ifndef CLIENT_H
#define CLIENT_H
#include <string>
#include <vector>

#include "../common_src/nitro_message.h"

#include "client_protocol.h"

class Client {
private:
    ClientProtocol clientProtocol;
    bool connected;

public:
    Client(const std::string& hostname, const std::string& port);
    void run();
    void readMessages(int count);
    void processCommand(const std::string& command);
    ~Client();
};

#endif
