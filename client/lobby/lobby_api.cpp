#include "lobby_api.h"

bool LobbyApi::login(const std::string& username) {
    proto.sendUsername(username);
    return proto.receiveActionCode() == ActionCode::USERNAME_OK;
}

std::vector<std::string> LobbyApi::listRooms() {
    proto.sendListRooms();
    return proto.receiveRoomList();
}

bool LobbyApi::createRoom(const std::string& code, unsigned maxPlayers) {
    proto.sendCreateRoom(code, maxPlayers);
    return proto.receiveActionCode() == ActionCode::ROOM_CREATED;
}

bool LobbyApi::joinRoom(const std::string& code) {
    proto.sendJoinRoom(code);
    return proto.receiveActionCode() == ActionCode::JOIN_OK;
}

PlayersList LobbyApi::listPlayers() {
    proto.sendListPlayers();
    auto items = proto.receiveRoomList();
    PlayersList snap;
    if (!items.empty() && items[0].rfind("maxPlayers:", 0) == 0) {
        std::string maxStr = items[0].substr(std::string("maxPlayers:").size());
        try {
            snap.maxPlayers = static_cast<unsigned>(std::stoul(maxStr));
        } catch (...) {
            snap.maxPlayers = 0;
        }
        items.erase(items.begin());
    }
    snap.players = std::move(items);
    return snap;
}

bool LobbyApi::pollStarted() {
    proto.sendListState();
    auto v = proto.receiveRoomList();
    return !v.empty() && v[0] == std::string("started");
}

bool LobbyApi::chooseCar(const std::string& carKey) {
    proto.sendChooseCar(carKey);
    return proto.receiveActionCode() == ActionCode::CHOOSE_CAR_OK;
}

void LobbyApi::startGame() { proto.sendStartGame(); }
