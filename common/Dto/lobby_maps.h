#ifndef LOBBY_MAPS_DTO_H
#define LOBBY_MAPS_DTO_H

#include <vector>
#include <string>
#include "dto.h"

class MapsDto : public Dto {
public:
    std::vector<std::string> selectedMaps;

    explicit MapsDto(uint8_t code) : Dto(code) {}
    MapsDto(uint8_t code, const std::vector<std::string>& maps) : Dto(code), selectedMaps(maps) {}
};

#endif