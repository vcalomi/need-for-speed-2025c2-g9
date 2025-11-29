#ifndef LOBBY_MAPS_DTO_H
#define LOBBY_MAPS_DTO_H

#include <vector>
#include "dto.h"

class MapsDto : public Dto {
public:
    std::vector<uint8_t> selectedMaps;

    explicit MapsDto(uint8_t code) : Dto(code) {}
    MapsDto(uint8_t code, const std::vector<uint8_t>& maps) : Dto(code), selectedMaps(maps) {}
    ~MapsDto() override = default;
};

#endif