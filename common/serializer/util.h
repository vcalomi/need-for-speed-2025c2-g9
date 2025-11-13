#ifndef UTIL_H
#define UTIL_H

#include <cstdint>
#include <cstring>
#include <vector>

#include <netinet/in.h>

namespace SerializerUtils {

inline void writeInt(std::vector<uint8_t>& buffer, size_t& pos, int value) {
    value = htonl(value);
    std::memcpy(&buffer[pos], &value, sizeof(int));
    pos += sizeof(int);
}

inline void writeFloat(std::vector<uint8_t>& buffer, size_t& pos, float value) {
    std::memcpy(&buffer[pos], &value, sizeof(float));
    pos += sizeof(float);
}

inline void writeByte(std::vector<uint8_t>& buffer, size_t& pos, uint8_t value) {
    buffer[pos++] = value;
}

inline void writeString(std::vector<uint8_t>& buffer, size_t& pos, const std::string& str) {
    writeByte(buffer, pos, static_cast<uint8_t>(str.length()));
    for (char c: str) {
        writeByte(buffer, pos, static_cast<uint8_t>(c));
    }
}

inline void writeBool(std::vector<uint8_t>& buffer, size_t& pos, bool value) {
    writeByte(buffer, pos, value ? 1 : 0);
}

inline int readInt(const std::vector<uint8_t>& buffer, size_t& pos) {
    int value;
    std::memcpy(&value, &buffer[pos], sizeof(int));
    pos += sizeof(int);
    return ntohl(value);
}

inline float readFloat(const std::vector<uint8_t>& buffer, size_t& pos) {
    float value;
    std::memcpy(&value, &buffer[pos], sizeof(float));
    pos += sizeof(float);
    return value;
}

inline uint8_t readByte(const std::vector<uint8_t>& buffer, size_t& pos) { return buffer[pos++]; }

inline std::string readString(const std::vector<uint8_t>& buffer, size_t& pos) {
    uint8_t length = readByte(buffer, pos);
    std::string str;
    str.reserve(length);
    for (int i = 0; i < length && pos < buffer.size(); i++) {
        str += static_cast<char>(readByte(buffer, pos));
    }
    return str;
}

inline bool readBool(const std::vector<uint8_t>& buffer, size_t& pos) {
    return readByte(buffer, pos) != 0;
}

};

#endif