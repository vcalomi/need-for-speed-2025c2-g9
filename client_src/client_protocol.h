#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <string>
#include <vector>

#include "../common_src/common_protocol.h"
#include "../common_src/socket.h"

class ClientProtocol {
private:
    Socket socket;
    CommonProtocol protocol;

public:
    ClientProtocol(const std::string& hostname, const std::string& port);
    void sendNitro();
    std::vector<uint8_t> receiveMessage();
    ~ClientProtocol();
};

#endif
