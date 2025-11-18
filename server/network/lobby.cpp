#include "lobby.h"

#include <iostream>
#include <utility>

#include "../../common/common_codes.h"
#include "../../common/Dto/lobby_auth.h"
#include "../../common/Dto/lobby_room.h"
#include "../../common/Dto/lobby_room_state.h"
#include "../../common/Dto/lobby_choose_car.h"

Lobby::Lobby(ServerProtocol& protocol, GameMonitor& gameMonitor, int clientId,
             Queue<std::shared_ptr<Dto>>& senderQueue,
             std::function<void(std::shared_ptr<GameRoom> room)> onStartGame):
        protocol(protocol),
        gameMonitor(gameMonitor),
        clientId(clientId),
        senderQueue(senderQueue),
        onStartGame(std::move(onStartGame)) {
    initHandlers();
}

void Lobby::run() {
    try {
        while (should_keep_running()) {
            lastDto = protocol.receiveDTO();
            if (!lastDto) {
                continue;
            }
            ActionCode action = static_cast<ActionCode>(lastDto->return_code());
            if (!should_keep_running()) {
                break;
            }
            bool handled = dispatcher.dispatch(action);
            if (!handled) {
                auto err = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_ERROR_MSG));
                err->errorMsg = "Unhandled action";
                protocol.sendDTO(err);
            }
        }
    } catch (const std::exception& e) {
        this->stop();
        return;
    }
}

void Lobby::initHandlers() {
    dispatcher.registerHandler(ActionCode::LIST_ROOMS, [this] { handleListRooms(); });
    dispatcher.registerHandler(ActionCode::CREATE_ROOM, [this] { handleCreateRoom(); });
    dispatcher.registerHandler(ActionCode::JOIN_ROOM, [this] { handleJoinRoom(); });
    dispatcher.registerHandler(ActionCode::CHOOSE_CAR, [this] { handleChooseCar(); });
    dispatcher.registerHandler(ActionCode::SEND_USERNAME, [this] { handleSendUsername(); });
    dispatcher.registerHandler(ActionCode::START_GAME, [this] { handleStartGame(); });
    dispatcher.registerHandler(ActionCode::LIST_PLAYERS, [this] { handleListPlayers(); });
    dispatcher.registerHandler(ActionCode::LIST_STATE, [this] { handleListState(); });
}

void Lobby::handleListRooms() {
    auto rooms = gameMonitor.getAvailableRooms();
    auto dto = std::make_shared<RoomDto>(static_cast<uint8_t>(ActionCode::LIST_ROOMS));
    dto->rooms = std::move(rooms);
    protocol.sendDTO(dto);
}

void Lobby::handleCreateRoom() {
    auto in = std::dynamic_pointer_cast<RoomDto>(lastDto);
    if (!in) {
        auto err = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_ERROR_MSG));
        err->errorMsg = "Invalid payload";
        protocol.sendDTO(err);
        return;
    }
    std::string roomName = in->roomCode;
    int maxPlayers = static_cast<int>(in->maxPlayers);
    if (gameMonitor.createGameRoom(roomName, clientId, senderQueue, maxPlayers)) {
        auto out = std::make_shared<RoomDto>(static_cast<uint8_t>(ActionCode::ROOM_CREATED));
        protocol.sendDTO(out);
    } else {
        auto err = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_ERROR_MSG));
        err->errorMsg = "Create room failed";
        protocol.sendDTO(err);
    }
}

void Lobby::handleJoinRoom() {
    auto in = std::dynamic_pointer_cast<RoomDto>(lastDto);
    if (!in) {
        auto err = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_ERROR_MSG));
        err->errorMsg = "Invalid payload";
        protocol.sendDTO(err);
        return;
    }
    if (gameMonitor.joinGameRoom(in->roomCode, clientId, senderQueue)) {
        auto out = std::make_shared<RoomDto>(static_cast<uint8_t>(ActionCode::JOIN_OK));
        protocol.sendDTO(out);
    } else {
        auto err = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_ERROR_MSG));
        err->errorMsg = "Join room failed";
        protocol.sendDTO(err);
    }
}

void Lobby::handleStartGame() {
    if (gameMonitor.startGameByClientId(clientId)) {
        stop();
    } else {
        auto err = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_ERROR_MSG));
        err->errorMsg = "Start game failed";
        protocol.sendDTO(err);
    }
}

void Lobby::handleChooseCar() {
    auto in = std::dynamic_pointer_cast<ChooseCarDto>(lastDto);
    std::string carName = in ? in->carKey : std::string();
    std::cout << "[Lobby] CHOOSE_CAR received: clientId=" << clientId << " carType='" << carName
              << "'" << std::endl;
    CarConfig car{};
    car.carType = carName;
    if (gameMonitor.chooseCarByClientId(clientId, car)) {
        auto out = std::make_shared<ChooseCarDto>(static_cast<uint8_t>(ActionCode::CHOOSE_CAR_OK));
        protocol.sendDTO(out);
    } else {
        auto err = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_ERROR_MSG));
        err->errorMsg = "Choose car failed";
        protocol.sendDTO(err);
    }
}

void Lobby::handleSendUsername() {
    auto in = std::dynamic_pointer_cast<AuthDto>(lastDto);
    std::string username = in ? in->username : std::string();
    bool ok = gameMonitor.setUsername(clientId, username);
    if (ok) {
        auto out = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::USERNAME_OK));
        protocol.sendDTO(out);
    } else {
        auto err = std::make_shared<AuthDto>(static_cast<uint8_t>(ActionCode::SEND_ERROR_MSG));
        err->errorMsg = "Username already used";
        protocol.sendDTO(err);
    }
}

void Lobby::handleListPlayers() {
    auto items = gameMonitor.getPlayersInRoomByClient(clientId);
    uint8_t maxPlayers = 0;
    std::vector<std::string> players;
    if (!items.empty() && items[0].rfind("maxPlayers:", 0) == 0) {
        std::string maxStr = items[0].substr(std::string("maxPlayers:").size());
        try {
            maxPlayers = static_cast<uint8_t>(std::stoul(maxStr));
        } catch (...) {
            maxPlayers = 0;
        }
        items.erase(items.begin());
    }
    players = std::move(items);
    auto dto = std::make_shared<RoomStateDto>(static_cast<uint8_t>(ActionCode::LIST_PLAYERS));
    dto->maxPlayers = maxPlayers;
    dto->players = std::move(players);
    protocol.sendDTO(dto);
}

void Lobby::handleListState() {
    bool started = gameMonitor.isGameStartedByClient(clientId);
    auto dto = std::make_shared<RoomStateDto>(static_cast<uint8_t>(ActionCode::LIST_STATE));
    dto->started = started;
    protocol.sendDTO(dto);
    if (started) {
        auto room = gameMonitor.getRoomByClient(clientId);
        if (room && onStartGame) {
            onStartGame(room);
        }
        stop();
    }
}
