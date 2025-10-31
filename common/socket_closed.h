#ifndef SOCKET_CLOSED
#define SOCKET_CLOSED
#include <exception>
#include <iostream>
#include <string>

struct SocketClosed: public std::runtime_error {
    SocketClosed(): std::runtime_error("Socket is closed") {}
};

#endif
