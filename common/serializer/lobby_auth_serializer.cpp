#include "lobby_auth_serializer.h"

#include <memory>
#include <vector>

#include "util.h"

std::vector<uint8_t> AuthSerializer::serialize(const Dto& dto) const {
    const AuthDto& ad = static_cast<const AuthDto&>(dto);
    size_t size = 0;
    switch (code_) {
        case ActionCode::SEND_USERNAME: {
            size = SerializerUtils::STRING_LENGTH_SIZE + ad.username.size();
            std::vector<uint8_t> buffer(size);
            size_t pos = 0;
            SerializerUtils::writeString(buffer, pos, ad.username);
            return buffer;
        }
        case ActionCode::USERNAME_OK: {
            return {};
        }
        case ActionCode::SEND_ERROR_MSG: {
            size = SerializerUtils::STRING_LENGTH_SIZE + ad.errorMsg.size();
            std::vector<uint8_t> buffer(size);
            size_t pos = 0;
            SerializerUtils::writeString(buffer, pos, ad.errorMsg);
            return buffer;
        }
        default:
            return {};
    }
}

std::shared_ptr<Dto> AuthSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;
    switch (code_) {
        case ActionCode::SEND_USERNAME: {
            std::string username = SerializerUtils::readString(buffer, pos);
            return std::make_shared<AuthDto>(static_cast<uint8_t>(code_), username);
        }
        case ActionCode::USERNAME_OK: {
            return std::make_shared<AuthDto>(static_cast<uint8_t>(code_));
        }
        case ActionCode::SEND_ERROR_MSG: {
            std::string msg = SerializerUtils::readString(buffer, pos);
            auto dto = std::make_shared<AuthDto>(static_cast<uint8_t>(code_));
            dto->errorMsg = std::move(msg);
            return dto;
        }
        default:
            return std::make_shared<AuthDto>(static_cast<uint8_t>(code_));
    }
}
