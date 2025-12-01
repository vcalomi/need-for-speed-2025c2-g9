#include "infinite_health_serializer.h"

#include <cstring>
#include <memory>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "util.h"

std::vector<uint8_t> InfiniteHealthSerializer::serialize(const Dto& dto) const {
    const InfiniteHealthDto& infiniteHealthDto = static_cast<const InfiniteHealthDto&>(dto);
    size_t size = infiniteHealthDto.username.length();
    std::vector<uint8_t> buffer(SerializerUtils::STRING_LENGTH_SIZE + size);
    size_t pos = 0;

    SerializerUtils::writeString(buffer, pos, infiniteHealthDto.username);
    return buffer;
}

std::shared_ptr<Dto> InfiniteHealthSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;
    std::string username = SerializerUtils::readString(buffer, pos);
    return std::make_shared<InfiniteHealthDto>(username);
}
