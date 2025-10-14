#ifndef MESSAGE_PARSER_H
#define MESSAGE_PARSER_H

#include <cstdint>
#include <vector>

#include "../common_src/nitro_message.h"

class MessageParser {

public:
    MessageParser();
    NitroMessage createNitroMessage(uint16_t carsWithActiveNitro, bool activated);
};

#endif
