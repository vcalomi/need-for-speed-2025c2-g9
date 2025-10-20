#include "protocol.h"

#include <array>
#include <cstring>
#include <stdexcept>

#include <arpa/inet.h>

Protocol::Protocol(Socket& s): skt(s) {}

void Protocol::send_server_event(uint16_t cars_with_nitro, uint8_t evt_kind) {
    std::array<uint8_t, 4> buf{};

    buf[0] = MSG_EVT;

    uint16_t cars_be = htons(cars_with_nitro);
    std::memcpy(&buf[1], &cars_be, sizeof(cars_be));

    buf[3] = evt_kind;

    int sent = skt.sendall(buf.data(), buf.size());
    if (sent != (int)buf.size()) {
        throw std::runtime_error("server_event: envío incompleto");
    }
}

void Protocol::send_client_event() {
    const uint8_t cmd = CMD_NITRO;
    const int sent = skt.sendall(&cmd, 1);
    if (sent != 1) {
        throw std::runtime_error("client_event: envío incompleto");
    }
}

Event Protocol::recv_event() {
    std::array<uint8_t, 4> buf{};
    int recvd = skt.recvall(buf.data(), buf.size());
    if (recvd != static_cast<int>(buf.size())) {
        throw std::runtime_error("recv_event: lectura incompleta");
    }
    uint16_t net_u16;
    std::memcpy(&net_u16, &buf[1], sizeof(net_u16));
    Event ev{};
    ev.cars_with_nitro = ntohs(net_u16);
    ev.kind = buf[3];
    return ev;
}


uint8_t Protocol::recv_client_event() {
    uint8_t cmd{};
    int recvd = skt.recvall(&cmd, 1);
    if (recvd != 1) {
        throw std::runtime_error("recv_client_event: lectura incompleta");
    }
    return cmd;
}
