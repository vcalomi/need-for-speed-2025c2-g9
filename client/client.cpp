
#include <iostream>

#include "../common/socket.h"

int main(int argc, char* argv[]) {

    std::cout << argc << std::endl;
    const char* port = argv[2];
    const char* hostname = argv[1];

    Socket cliente(hostname, port);

    return 0;
}
