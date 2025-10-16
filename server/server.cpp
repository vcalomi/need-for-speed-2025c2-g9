#include "./server.h"

#include "../common/socket.h"
#include "./acceptor.h"


void Server::run(const char* port) { Acceptor client_acceptor(port); }

int main(int argc, char** argv) {
    Server server;
    server.run(argv[1]);
}
