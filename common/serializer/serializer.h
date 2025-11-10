#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <cstdint>
#include <memory>
#include <vector>

#include "../Dto/dto.h"

class Serializer {
public:
    virtual std::vector<uint8_t> serialize(const Dto& dto) const;
    virtual std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const;
    virtual ~Serializer();
};

#endif
