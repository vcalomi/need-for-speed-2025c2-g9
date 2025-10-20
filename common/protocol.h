#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <array>
#include <cstdint>

#include "command.h"
#include "event.h"
#include "socket.h"


class Protocol {
private:
    Socket& skt;

public:
    explicit Protocol(Socket& s);


    void send_client_event();
    void send_server_event(uint16_t cars_with_nitro, uint8_t evt_kind);

    uint8_t recv_client_event();
    Event recv_event();
};

#endif
