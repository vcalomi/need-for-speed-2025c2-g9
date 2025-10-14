#include "message_parser.h"

#include <vector>
#include <cstring>
#include "../common_src/common_codes.h"
#include <netinet/in.h>

MessageParser::MessageParser() {}

std::vector<uint8_t> MessageParser::createNitroMessage(uint16_t carsWithActiveNitro, bool activated) {
    std::vector<uint8_t> msg;
    msg.resize(4);
    msg[0] = uint8_t(ActionCode::SERVER_MSG);
    uint16_t parsed = htons(carsWithActiveNitro);
    std::memcpy(&msg[1], &parsed, sizeof(parsed));

    msg[3] = activated ? uint8_t(ActionCode::NITRO_ACTIVATED) :
                         uint8_t(ActionCode::NITRO_DEACTIVATED);

    return msg;
    // NitroMessage msg;
    // msg.carsWithNitro = uint16_t(carsWithActiveNitro);
    // msg.nitroState = activated;
    // return msg
}
