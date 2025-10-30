#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <vector>
#include <cstdint>

#include "../Dto/dto.h"

class Serializer {
public:
    virtual std::vector<uint8_t> serialize(Dto dto) const;
    virtual Dto deserialize(const std::vector<uint8_t>& buffer) const;
    virtual int getSize() const;
    virtual ~Serializer();
};

#endif