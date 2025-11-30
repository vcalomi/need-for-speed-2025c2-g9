#ifndef INFINITE_HEALTH_SERIALIZER_H
#define INFINITE_HEALTH_SERIALIZER_H

#include <memory>
#include <vector>

#include "../Dto/infinite_health.h"

#include "serializer.h"

class InfiniteHealthSerializer: public Serializer {

public:
    InfiniteHealthSerializer() {}
    std::vector<uint8_t> serialize(const Dto&) const override {
        std::vector<uint8_t> buffer(0);
        return buffer;
    }
    std::shared_ptr<Dto> deserialize(const std::vector<uint8_t>&) const override {
        return std::make_shared<InfiniteHealthDto>();
    }
};

#endif
