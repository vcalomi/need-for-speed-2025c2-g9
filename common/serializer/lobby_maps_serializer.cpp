#include "lobby_maps_serializer.h"

#include <memory>
#include <vector>

#include "util.h"

std::vector<uint8_t> MapsSerializer::serialize(const Dto& dto) const {
    const MapsDto& mapsDto = static_cast<const MapsDto&>(dto);
    size_t buffer_size = 1 + SerializerUtils::calculateStringVectorSize(mapsDto.selectedMaps);
    std::vector<uint8_t> buffer(buffer_size);
    size_t pos = 0;
    SerializerUtils::writeStringVector(buffer, pos, mapsDto.selectedMaps);
    return buffer;
}

std::shared_ptr<Dto> MapsSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;
    auto dto = std::make_shared<MapsDto>(static_cast<uint8_t>(code));
    dto->selectedMaps = SerializerUtils::readStringVector(buffer, pos);
    return dto;
}
