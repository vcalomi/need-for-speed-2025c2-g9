#ifndef LOBBY_API_H
#define LOBBY_API_H

#include <string>
#include <utility>
#include <vector>

#include "../network/client_protocol.h"

struct PlayersList {
    unsigned maxPlayers = 0;
    std::vector<std::string> players;
};

class LobbyApi {
public:
    explicit LobbyApi(ClientProtocol& proto): proto(proto) {}

    bool login(const std::string& username);
    std::vector<std::string> listRooms();
    bool createRoom(const std::string& code, unsigned maxPlayers);
    bool joinRoom(const std::string& code);
    PlayersList listPlayers();
    bool pollStarted();
    bool chooseCar(const std::string& carKey);
    void startGame();

private:
    ClientProtocol& proto;
};

#endif
