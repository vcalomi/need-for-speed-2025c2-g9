#include <iostream>

#include "server.h"

#define NUMBER_ARG 2
#define MSG_ERROR_STDIN "Command line: <port>"
#define ARG_INDEX_PORT 1

int main(int argc, char* argv[]) {
    if (argc != NUMBER_ARG) {
        std::cout << MSG_ERROR_STDIN << std::endl;
        return 1;
    }
    std::string port = std::string(argv[ARG_INDEX_PORT]);
    try {
        Server server(port);
        server.run();
        return 0;
    } catch (...) {
        return 1;
    }
}
