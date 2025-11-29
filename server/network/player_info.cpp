#include "player_info.h"

PlayerInfo::PlayerInfo(int id, const std::string& user, const std::string& car, 
                       std::shared_ptr<ServerProtocol> prot):
        player_id(id),
        username(user),
        car_type(car),
        protocol(prot) {}
