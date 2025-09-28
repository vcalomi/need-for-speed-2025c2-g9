#ifndef CLIENT_ACCEPTOR_H
#define CLIENT_ACCEPTOR_H

#include <string>
#include <vector>

#include "../common_src/client_command.h"
#include "../common_src/liberror.h"
#include "../common_src/socket.h"
#include "../common_src/thread.h"

#include "client_handler.h"
#include "client_monitor.h"

class ClientAcceptor: public Thread {
private:
    ClientMonitor& clientMonitor;
    Queue<ClientCommand>& gameLoopQueue;
    Socket acceptor;
    std::atomic<int> nextClientId;
    std::vector<ClientHandler*> clients;

public:
    ClientAcceptor(const std::string& port, ClientMonitor& monitor, Queue<ClientCommand>& queue);
    void run() override;
    void close();
    void reap();
    void clear();
    ~ClientAcceptor();
};

#endif
