#include "lobby_api.h"

#include "../../common/Dto/lobby_auth.h"
#include "../../common/Dto/lobby_room.h"
#include "../../common/Dto/lobby_room_state.h"
#include "../../common/Dto/lobby_choose_car.h"
#include "../../common/Dto/lobby_maps.h"

bool LobbyApi::login(const std::string& username) {
    auto req = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_USERNAME), username);
    proto.sendDTO(req);
    auto resp = proto.receiveDTO();
    return resp && static_cast<ActionCode>(resp->return_code()) == ActionCode::USERNAME_OK;
}

std::vector<std::string> LobbyApi::listRooms() {
    auto req = std::make_shared<RoomDto>(static_cast<uint8_t>(ActionCode::LIST_ROOMS));
    proto.sendDTO(req);
    auto resp = proto.receiveDTO();
    if (!resp || static_cast<ActionCode>(resp->return_code()) != ActionCode::LIST_ROOMS) return {};
    auto dto = std::dynamic_pointer_cast<RoomDto>(resp);
    if (!dto) return {};
    return dto->rooms;
}

bool LobbyApi::createRoom(const std::string& code, unsigned maxPlayers) {
    auto req = std::make_shared<RoomDto>(static_cast<uint8_t>(ActionCode::CREATE_ROOM));
    req->roomCode = code;
    req->maxPlayers = static_cast<uint8_t>(maxPlayers);
    proto.sendDTO(req);
    auto resp = proto.receiveDTO();
    return resp && static_cast<ActionCode>(resp->return_code()) == ActionCode::ROOM_CREATED;
}

bool LobbyApi::joinRoom(const std::string& code) {
    auto req = std::make_shared<RoomDto>(static_cast<uint8_t>(ActionCode::JOIN_ROOM));
    req->roomCode = code;
    proto.sendDTO(req);
    auto resp = proto.receiveDTO();
    return resp && static_cast<ActionCode>(resp->return_code()) == ActionCode::JOIN_OK;
}

PlayersList LobbyApi::listPlayers() {
    PlayersList snap;
    auto req = std::make_shared<RoomStateDto>(static_cast<uint8_t>(ActionCode::LIST_PLAYERS));
    proto.sendDTO(req);
    auto resp = proto.receiveDTO();
    if (!resp || static_cast<ActionCode>(resp->return_code()) != ActionCode::LIST_PLAYERS) return snap;
    auto dto = std::dynamic_pointer_cast<RoomStateDto>(resp);
    if (!dto) return snap;
    snap.maxPlayers = dto->maxPlayers;
    snap.players = dto->players;
    return snap;
}

bool LobbyApi::pollStarted() {
    auto req = std::make_shared<RoomStateDto>(static_cast<uint8_t>(ActionCode::LIST_STATE));
    proto.sendDTO(req);
    auto resp = proto.receiveDTO();
    if (!resp) {
        return false;
    }
    if (static_cast<ActionCode>(resp->return_code()) != ActionCode::LIST_STATE) {
        return false;
    }
    auto dto = std::dynamic_pointer_cast<RoomStateDto>(resp);
    if (!dto) {
        return false;
    }
    return dto->started;
}

bool LobbyApi::chooseCar(const std::string& carKey) {
    auto req = std::make_shared<ChooseCarDto>(static_cast<uint8_t>(ActionCode::CHOOSE_CAR));
    req->carKey = carKey;
    proto.sendDTO(req);
    auto resp = proto.receiveDTO();
    return resp && static_cast<ActionCode>(resp->return_code()) == ActionCode::CHOOSE_CAR_OK;
}

void LobbyApi::startGame() {
    auto req = std::make_shared<RoomStateDto>(static_cast<uint8_t>(ActionCode::START_GAME));
    proto.sendDTO(req);
}

bool LobbyApi::selectMaps(const std::vector<std::string>& mapNames) {
    auto req = std::make_shared<MapsDto>(static_cast<uint8_t>(ActionCode::SELECT_MAPS));
    req->selectedMaps = mapNames;
    proto.sendDTO(req);
    auto resp = proto.receiveDTO();
    return resp && static_cast<ActionCode>(resp->return_code()) == ActionCode::SELECT_MAPS_OK;
}
