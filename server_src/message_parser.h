#ifndef MESSAGE_PARSER_H
#define MESSAGE_PARSER_H

#include <vector>
#include <cstdint>
#include "../common_src/nitro_message.h"

class MessageParser {

public:
    MessageParser();
    std::vector<uint8_t> createNitroMessage(uint16_t carsWithActiveNitro, bool activated);
};

#endif
