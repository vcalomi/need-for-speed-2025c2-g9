#include "lobby.h"

#include <iostream>
#include <utility>

#include "../common/common_codes.h"

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
            ActionCode action = protocol.receiveActionCode();
            if (!should_keep_running()) {
                break;
            }
            bool handled = dispatcher.dispatch(action);
            if (!handled) {
                protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
            }
        }
    } catch (const std::exception& e) {
        this->stop();
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
    protocol.sendRoomList(rooms);
}

void Lobby::handleCreateRoom() {
    std::string roomName = protocol.receiveRoomName();
    int maxPlayers = protocol.receiveMaxPlayers();
    if (gameMonitor.createGameRoom(roomName, clientId, senderQueue, maxPlayers)) {
        protocol.sendMsg({ActionCode::ROOM_CREATED});
    } else {
        protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
    }
}

void Lobby::handleJoinRoom() {
    std::string roomName = protocol.receiveRoomName();
    if (gameMonitor.joinGameRoom(roomName, clientId, senderQueue)) {
        protocol.sendMsg({ActionCode::JOIN_OK});
    } else {
        protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
    }
}

void Lobby::handleStartGame() {
    if (gameMonitor.startGameByClientId(clientId)) {
        stop();
    } else {
        protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
    }
}

void Lobby::handleChooseCar() {
    std::string carType = protocol.receiveRoomName();
    CarConfig car{};
    car.carType = carType;
    if (gameMonitor.chooseCarByClientId(clientId, car)) {
        protocol.sendMsg({ActionCode::CHOOSE_CAR_OK});
    } else {
        protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
    }
}

void Lobby::handleSendUsername() {
    std::string username = protocol.receiveRoomName();
    gameMonitor.setUsername(clientId, username);
}

void Lobby::handleListPlayers() {
    auto players = gameMonitor.getPlayersInRoomByClient(clientId);
    protocol.sendRoomList(players);
}

void Lobby::handleListState() {
    bool started = gameMonitor.isGameStartedByClient(clientId);
    protocol.sendRoomList({started ? std::string("started") : std::string("waiting")});
    if (started) {
        auto room = gameMonitor.getRoomByClient(clientId);
        if (room && onStartGame) {
            onStartGame(room);
        }
        stop();
    }
}
#include <string>
