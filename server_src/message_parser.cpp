#include "message_parser.h"

#include <cstring>
#include <vector>

#include <netinet/in.h>

#include "../common_src/common_codes.h"

MessageParser::MessageParser() {}

NitroMessage MessageParser::createNitroMessage(uint16_t carsWithActiveNitro, bool activated) {
    NitroMessage msg;
    msg.msgCode = ActionCode::SERVER_MSG;
    msg.carsWithNitro = uint16_t(carsWithActiveNitro);
    msg.nitroState = activated ? ActionCode::NITRO_ACTIVATED : ActionCode::NITRO_DEACTIVATED;
    return msg;
}
