#include "./server.h"

#include <iostream>

#include "../common/socket.h"
#include "./acceptor.h"


void Server::run(const char* port) {
    Acceptor client_acceptor(port);
    client_acceptor.start();
}

int main(int argc, char** argv) {
    std::cout << argc << std::endl;
    Server server;
    server.run(argv[1]);
}
