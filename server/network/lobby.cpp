#include "lobby.h"

#include <iostream>
#include <utility>

#include "../../common/common_codes.h"
#include "../../common/Dto/lobby_auth.h"
#include "../../common/Dto/lobby_room.h"
#include "../../common/Dto/lobby_room_state.h"
#include "../../common/Dto/lobby_choose_car.h"
#include "../../common/Dto/lobby_maps.h"

Lobby::Lobby(Socket socket, GameMonitor& gameMonitor, int clientId):
        peer(std::move(socket)),
        gameMonitor(gameMonitor),
        clientId(clientId),
        game_started(false) {
    protocol = std::make_shared<ServerProtocol>(peer);
    initHandlers();
}

void Lobby::run() {
    try {
        while (should_keep_running() && !game_started) {
            auto dto = protocol->receiveDTO();
            if (!dto) continue;
            
            ActionCode action = static_cast<ActionCode>(dto->return_code());
            dispatcher.dispatch(action, dto);
        }
    } catch (const std::exception& e) {
        std::cerr << "Lobby error: " << e.what() << std::endl;
    }
}

void Lobby::initHandlers() {
    dispatcher.registerHandler(ActionCode::LIST_ROOMS, [this](std::shared_ptr<Dto> dto) { handleListRooms(dto); });
    dispatcher.registerHandler(ActionCode::CREATE_ROOM, [this](std::shared_ptr<Dto> dto) { handleCreateRoom(dto); });
    dispatcher.registerHandler(ActionCode::JOIN_ROOM, [this](std::shared_ptr<Dto> dto) { handleJoinRoom(dto); });
    dispatcher.registerHandler(ActionCode::CHOOSE_CAR, [this](std::shared_ptr<Dto> dto) { handleChooseCar(dto); });
    dispatcher.registerHandler(ActionCode::SEND_USERNAME, [this](std::shared_ptr<Dto> dto) { handleSendUsername(dto); });
    dispatcher.registerHandler(ActionCode::START_GAME, [this](std::shared_ptr<Dto> dto) { handleStartGame(dto); });
    dispatcher.registerHandler(ActionCode::LIST_PLAYERS, [this](std::shared_ptr<Dto> dto) { handleListPlayers(dto); });
    dispatcher.registerHandler(ActionCode::LIST_STATE, [this](std::shared_ptr<Dto> dto) { handleListState(dto); });
    dispatcher.registerHandler(ActionCode::SELECT_MAPS, [this](std::shared_ptr<Dto> dto) { handleSelectMaps(dto); });
}

void Lobby::handleListRooms(std::shared_ptr<Dto>) {
    auto rooms = gameMonitor.getAvailableRooms();
    auto response = std::make_shared<RoomDto>(static_cast<uint8_t>(ActionCode::LIST_ROOMS));
    response->rooms = std::move(rooms);
    protocol->sendDTO(response);
}

void Lobby::handleCreateRoom(std::shared_ptr<Dto> dto) {
    auto roomDto = std::dynamic_pointer_cast<RoomDto>(dto);
    if (!roomDto) {
        auto err = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_ERROR_MSG));
        err->errorMsg = "Invalid payload";
        protocol->sendDTO(err);
        return;
    }
    std::string roomName = roomDto->roomCode;
    int maxPlayers = static_cast<int>(roomDto->maxPlayers);
    Player* player = new Player(protocol, clientId);
    
    if (gameMonitor.createGameRoom(roomName, clientId, player, maxPlayers)) {
        auto out = std::make_shared<RoomDto>(static_cast<uint8_t>(ActionCode::ROOM_CREATED));
        protocol->sendDTO(out);
    } else {
        auto err = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_ERROR_MSG));
        err->errorMsg = "Create room failed";
        protocol->sendDTO(err);
    }
}

void Lobby::handleJoinRoom(std::shared_ptr<Dto> dto) {
    auto roomDto = std::dynamic_pointer_cast<RoomDto>(dto);
    if (!roomDto) {
        auto err = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_ERROR_MSG));
        err->errorMsg = "Invalid payload";
        protocol->sendDTO(err);
        return;
    }
    Player* player = new Player(protocol, clientId);
    
    if (gameMonitor.joinGameRoom(roomDto->roomCode, clientId, player)) {
        auto out = std::make_shared<RoomDto>(static_cast<uint8_t>(ActionCode::JOIN_OK));
        protocol->sendDTO(out);
    } else {
        auto err = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_ERROR_MSG));
        err->errorMsg = "Join room failed";
        protocol->sendDTO(err);
    }
}

void Lobby::handleSendUsername(std::shared_ptr<Dto> dto) {
    auto authDto = std::dynamic_pointer_cast<AuthDto>(dto);
    if (!authDto) {
        auto err = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_ERROR_MSG));
        err->errorMsg = "Invalid payload";
        protocol->sendDTO(err);
        return;
    }
    
     std::string username = authDto ? authDto->username : std::string();
    if (gameMonitor.setUsername(clientId, username)) {
        auto out = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::USERNAME_OK));
        protocol->sendDTO(out);
    } else {
        auto err = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_ERROR_MSG));
        err->errorMsg = "Username already taken";
        protocol->sendDTO(err);
    }
}

void Lobby::handleChooseCar(std::shared_ptr<Dto> dto) {
    auto carDto = std::dynamic_pointer_cast<ChooseCarDto>(dto);
    if (!carDto) {
        auto err = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_ERROR_MSG));
        err->errorMsg = "Invalid payload";
        protocol->sendDTO(err);
        return;
    }
    
    CarConfig config;
    config.carType = carDto->carKey;
    
    if (gameMonitor.chooseCarByClientId(clientId, config)) {
        auto out = std::make_shared<ChooseCarDto>(static_cast<uint8_t>(ActionCode::CHOOSE_CAR_OK));
        out->carKey = carDto->carKey;
        protocol->sendDTO(out);
    } else {
        auto err = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_ERROR_MSG));
        err->errorMsg = "Car selection failed";
        protocol->sendDTO(err);
    }
}

void Lobby::handleListPlayers(std::shared_ptr<Dto>) {
    auto room = gameMonitor.getRoomByClient(clientId);
    if (!room) {
        auto err = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_ERROR_MSG));
        err->errorMsg = "Not in a room";
        protocol->sendDTO(err);
        return;
    }
    
    auto players = gameMonitor.getPlayersInRoomByClient(clientId);
    auto response = std::make_shared<RoomStateDto>(static_cast<uint8_t>(ActionCode::LIST_PLAYERS));
    response->players = std::move(players);
    response->maxPlayers = room->getMaxPlayers();
    protocol->sendDTO(response);
}

void Lobby::handleListState(std::shared_ptr<Dto>) {
    auto room = gameMonitor.getRoomByClient(clientId);
    if (!room) {
        auto err = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_ERROR_MSG));
        err->errorMsg = "You must be in a room to start the game";
        protocol->sendDTO(err);
        return;
    }
    
    bool started = room->isInRace();
    auto response = std::make_shared<RoomStateDto>(static_cast<uint8_t>(ActionCode::LIST_STATE));
    response->started = started;
    protocol->sendDTO(response);
    
    if (started) {
        room->startGameForPlayer(clientId);
        game_started = true;
        stop();
    }
}

void Lobby::handleStartGame(std::shared_ptr<Dto>) {
    auto room = gameMonitor.getRoomByClient(clientId);
    if (!room) return;
    
    if (gameMonitor.startGameByClientId(clientId)) {
        room->startGameForPlayer(clientId);
        game_started = true;
        stop();
    } else {
        auto err = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_ERROR_MSG));
        err->errorMsg = "Cannot start game - need at least 1 player with car selected";
        protocol->sendDTO(err);
    }
}

void Lobby::handleSelectMaps(std::shared_ptr<Dto> dto) {
    auto mapsDto = std::dynamic_pointer_cast<MapsDto>(dto);
    if (!mapsDto) {
        auto err = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_ERROR_MSG));
        err->errorMsg = "Invalid payload";
        protocol->sendDTO(err);
        return;
    }

    auto room = gameMonitor.getRoomByClient(clientId);

    if (room) {
        room->setSelectedMaps(mapsDto->selectedMaps);
    }

    auto response = std::make_shared<MapsDto>(static_cast<uint8_t>(ActionCode::SELECT_MAPS_OK));
    response->selectedMaps = mapsDto->selectedMaps;
    protocol->sendDTO(response);
    
    std::cout << "SELECT_MAPS_OK sent to client" << std::endl;
}
