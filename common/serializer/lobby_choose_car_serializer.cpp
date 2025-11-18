#include "lobby_choose_car_serializer.h"

#include <memory>
#include <vector>

#include "util.h"

std::vector<uint8_t> ChooseCarSerializer::serialize(const Dto& dto) const {
    const ChooseCarDto& cc = static_cast<const ChooseCarDto&>(dto);
    switch (code_) {
        case ActionCode::CHOOSE_CAR: {
            size_t sz = SerializerUtils::STRING_LENGTH_SIZE + cc.carKey.size();
            std::vector<uint8_t> buf(sz);
            size_t pos = 0;
            SerializerUtils::writeString(buf, pos, cc.carKey);
            return buf;
        }
        case ActionCode::CHOOSE_CAR_OK: {
            return {};
        }
        default:
            return {};
    }
}

std::shared_ptr<Dto> ChooseCarSerializer::deserialize(const std::vector<uint8_t>& buffer) const {
    size_t pos = 0;
    switch (code_) {
        case ActionCode::CHOOSE_CAR: {
            std::string key = SerializerUtils::readString(buffer, pos);
            return std::make_shared<ChooseCarDto>(static_cast<uint8_t>(code_), key);
        }
        case ActionCode::CHOOSE_CAR_OK: {
            return std::make_shared<ChooseCarDto>(static_cast<uint8_t>(code_));
        }
        default:
            return std::make_shared<ChooseCarDto>(static_cast<uint8_t>(code_));
    }
}
