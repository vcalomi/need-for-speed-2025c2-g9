#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include <memory>
#include <string>
#include "server_protocol.h"

class PlayerInfo {
private:
    int player_id;
    std::string username;
    std::string car_type;
    std::shared_ptr<ServerProtocol> protocol;

public:
    PlayerInfo(int id, const std::string& user, const std::string& car, 
               std::shared_ptr<ServerProtocol> prot);
    
    int getPlayerId() const { return player_id; }
    std::string getUsername() const { return username; }
    std::string getCarType() const { return car_type; }
    std::shared_ptr<ServerProtocol> getProtocol() { return protocol; }
};

#endif
