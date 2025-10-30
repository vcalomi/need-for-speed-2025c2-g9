#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <string>
#include <vector>

#include "../common_src/client_command.h"
#include "../common_src/liberror.h"
#include "../common_src/socket.h"
#include "../common_src/thread.h"

#include "game_lobby.h"
#include "client_handler.h"

class Acceptor: public Thread {
private:
    Socket acceptor;
    GameLobby& gameLobby;
    std::atomic<int> nextClientId;

public:
    Acceptor(const std::string& port, GameLobby& gameLobby);
    void run() override;
    void close();
    ~Acceptor();
};

#endif
