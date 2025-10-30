#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "../common_src/common_protocol.h"
#include "../common_src/socket.h"
#include "../common_src/common_codes.h"
#include "../common_src/Dto/vehicle.h"
#include "../common_src/serializer/serializer.h"

class ClientProtocol {
private:
    Socket socket;
    CommonProtocol protocol;
    std::map<uint8_t, std::unique_ptr<Serializer>> serializers;

public:
    ClientProtocol(const std::string& hostname, const std::string& port);

    void sendCreateRoom(const std::string& roomName);
    void sendJoinRoom(const std::string& roomName);
    void sendListRooms();
    void sendStartGame();
    void sendChooseCar(const std::string& carType);
    std::vector<std::string> receiveRoomList();
    ActionCode receiveAction();
    std::shared_ptr<Dto> receiveDTO();
    ActionCode receiveActionCode();
    bool isClientConnected() const;
    void close();
    ~ClientProtocol();
};

#endif
