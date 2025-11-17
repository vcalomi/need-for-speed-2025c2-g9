#ifndef UTIL_H
#define UTIL_H

#include <cstdint>
#include <cstring>
#include <vector>

#include <netinet/in.h>
#include <string>

namespace SerializerUtils {

constexpr size_t STRING_LENGTH_SIZE = sizeof(int);

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
    writeInt(buffer, pos, static_cast<int>(str.length()));
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
    int length = readInt(buffer, pos);
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

inline void writeStringVector(std::vector<uint8_t>& buffer, size_t& pos, const std::vector<std::string>& vec) {
        writeInt(buffer, pos, static_cast<int>(vec.size()));
        for (const auto& str : vec) {
            writeString(buffer, pos, str);
        }
    }

inline std::vector<std::string> readStringVector(const std::vector<uint8_t>& buffer, size_t& pos) {
    int count = readInt(buffer, pos);
    std::vector<std::string> result;
    result.reserve(count);
    for (int i = 0; i < count; ++i) {
        result.push_back(readString(buffer, pos));
    }
    return result;
}

inline size_t calculateStringVectorSize(const std::vector<std::string>& vec) {
        size_t total_size = sizeof(int); // Para el count
        for (const auto& str : vec) {
            total_size += STRING_LENGTH_SIZE + str.length();
        }
        return total_size;
    }

};

#endif