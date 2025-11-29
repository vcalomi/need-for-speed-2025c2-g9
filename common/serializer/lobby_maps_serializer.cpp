#include "lobby_maps_serializer.h"

#include <memory>
#include <vector>

#include "util.h"

// Forzar la generación de la vtable
MapsSerializer::MapsSerializer(ActionCode code) : code(code) {}

MapsSerializer::~MapsSerializer() = default;

std::vector<uint8_t> MapsSerializer::serialize(const Dto& dto) const {
    const MapsDto& mapsDto = static_cast<const MapsDto&>(dto);
    size_t buffer_size = 1 + SerializerUtils::calculateByteVectorSize(mapsDto.selectedMaps);
    std::vector<uint8_t> buffer(buffer_size);
    size_t pos = 0;
    SerializerUtils::writeByteVector(buffer, pos, mapsDto.selectedMaps);
    return buffer;
}

std::shared_ptr<Dto> MapsSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;
    auto dto = std::make_shared<MapsDto>(static_cast<uint8_t>(code));
    dto->selectedMaps = SerializerUtils::readByteVector(buffer, pos);
    return dto;
}
