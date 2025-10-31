#include "serializer.h"

std::vector<uint8_t> Serializer::serialize(const Dto& dto) const {
    throw std::runtime_error("Serializer::serialize not implemented");
}

std::shared_ptr<Dto> Serializer::deserialize(const std::vector<uint8_t>& buffer) const {
    throw std::runtime_error("Serializer::deserialize not implemented");
}

int Serializer::getSize() const {
    throw std::runtime_error("Serializer::getSize not implemented");
}

Serializer::~Serializer() = default;