#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../../common/Dto/vehicle.h"
#include "../../common/common_codes.h"
#include "../../common/common_protocol.h"
#include "../../common/serializer/serializer.h"
#include "../../common/socket.h"

class ClientProtocol {
private:
    Socket socket;
    CommonProtocol protocol;
    std::map<uint8_t, std::unique_ptr<Serializer>> serializers;
    bool dummyMode = false;

public:
    ClientProtocol(); 
    ClientProtocol(const std::string& hostname, const std::string& port);

    bool isDummy() const { return dummyMode; }  // para verificar si es modo dummy

    void sendUsername(const std::string& username);
    void sendCreateRoom(const std::string& roomName, unsigned maxPlayers = 8);
    void sendJoinRoom(const std::string& roomName);
    void sendListRooms();
    void sendListPlayers();
    void sendListState();
    void sendStartGame();
    void sendChooseCar(const std::string& carType);
    std::vector<std::string> receiveRoomList();
    ActionCode receiveAction();
    std::shared_ptr<Dto> receiveDTO();
    void sendDTO(std::shared_ptr<Dto> dto);
    ActionCode receiveActionCode();
    bool isClientConnected() const;
    void close();
    ~ClientProtocol();
};

#endif
