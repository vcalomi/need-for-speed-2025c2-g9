#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <string>
#include <vector>

#include "../../common/liberror.h"
#include "../../common/socket.h"
#include "../../common/thread.h"

#include "game_monitor.h"
#include "lobby.h"

class Acceptor: public Thread {
private:
    Socket acceptor;
    GameMonitor& gameMonitor;
    std::atomic<int> nextClientId;
    std::vector<Lobby*> lobbies;

public:
    Acceptor(const std::string& port, GameMonitor& gameMonitor);
    void run() override;
    void close();
    void reap();
    void clear();
    ~Acceptor();
};

#endif
