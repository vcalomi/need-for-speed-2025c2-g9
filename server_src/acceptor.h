#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <string>
#include <vector>

#include "../common_src/client_command.h"
#include "../common_src/liberror.h"
#include "../common_src/socket.h"
#include "../common_src/thread.h"

#include "client_handler.h"

class Acceptor: public Thread {
private:
    Socket acceptor;
    std::atomic<int> nextClientId;
    std::vector<ClientHandler*> clients;

public:
    Acceptor(const std::string& port);
    void run() override;
    void close();
    void reap();
    void clear();
    ~Acceptor();
};

#endif
