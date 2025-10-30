#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <string>
#include <vector>

#include "../common_src/common_protocol.h"
#include "../common_src/socket.h"

class ClientProtocol {
private:
    Socket socket;
    CommonProtocol protocol;

public:
    ClientProtocol(const std::string& hostname, const std::string& port);

    void sendCreateRoom(const std::string& roomName);
    void sendJoinRoom(const std::string& roomName);
    void sendListRooms();
    void sendStartGame();
    void sendChooseCar(const std::string& carType);
    std::vector<std::string> receiveRoomList();
    void close();
    ~ClientProtocol();
};

#endif
