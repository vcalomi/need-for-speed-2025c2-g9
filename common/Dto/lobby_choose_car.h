#ifndef LOBBY_CHOOSE_CAR_DTO_H
#define LOBBY_CHOOSE_CAR_DTO_H

#include <string>
#include "dto.h"

class ChooseCarDto : public Dto {
public:
    std::string carKey;
    explicit ChooseCarDto(uint8_t code) : Dto(code) {}
    ChooseCarDto(uint8_t code, const std::string& key) : Dto(code), carKey(key) {}
};

#endif
