#pragma once

#include "../common/socket.h"
#include "../common/thread.h"

class Acceptor: public Thread {

private:
    Socket listen_socket_;

public:
    explicit Acceptor(const char* port);
    ~Acceptor();
    void run() override;
}
