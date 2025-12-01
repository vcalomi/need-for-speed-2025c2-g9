#ifndef LOBBY_MAPAS_SERIALIZER_H
#define LOBBY_MAPAS_SERIALIZER_H

#include "serializer.h"
#include "../common_codes.h"
#include "../Dto/lobby_maps.h"

class MapsSerializer : public Serializer {
    ActionCode code;
public:
    explicit MapsSerializer(ActionCode code) : code(code) {}
    std::vector<uint8_t> serialize(const Dto& dto) const override;
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const override;
};

#endif
