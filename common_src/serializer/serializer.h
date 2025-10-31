#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <vector>
#include <cstdint>
#include <memory>

#include "../Dto/dto.h"

class Serializer {
public:
    virtual std::vector<uint8_t> serialize(const Dto& dto) const;
    virtual std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>& buffer) const;
    virtual int getSize() const;
    virtual ~Serializer();
};

#endif