#include "lobby_worker.h"

#include <iostream>
#include <utility>

#include "../common/common_codes.h"

LobbyWorker::LobbyWorker(ServerProtocol& protocol,
                         GameLobby& lobby,
                         int clientId,
                         Queue<std::shared_ptr<Dto>>& senderQueue,
                         std::function<void()> onStartGame)
    : protocol(protocol), lobby(lobby), clientId(clientId), senderQueue(senderQueue), onStartGame(std::move(onStartGame)) {
    initHandlers();
}

void LobbyWorker::run() {
    try {
        while (should_keep_running()) {
            ActionCode action = protocol.receiveActionCode();
            if (!dispatcher.dispatch(action)) {
                protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
            }
        }
    } catch (const std::exception& e) {}
}

void LobbyWorker::initHandlers() {
    dispatcher.registerHandler(ActionCode::LIST_ROOMS, [this]{ handleListRooms(); });
    dispatcher.registerHandler(ActionCode::CREATE_ROOM, [this]{ handleCreateRoom(); });
    dispatcher.registerHandler(ActionCode::JOIN_ROOM, [this]{ handleJoinRoom(); });
    dispatcher.registerHandler(ActionCode::CHOOSE_CAR, [this]{ handleChooseCar(); });
    dispatcher.registerHandler(ActionCode::SEND_USERNAME, [this]{ handleSendUsername(); });
    dispatcher.registerHandler(ActionCode::START_GAME, [this]{ handleStartGame(); });
    dispatcher.registerHandler(ActionCode::LIST_PLAYERS, [this]{ handleListPlayers(); });
    dispatcher.registerHandler(ActionCode::LIST_STATE, [this]{ handleListState(); });
}

void LobbyWorker::handleListRooms() {
    auto rooms = lobby.getAvailableRooms();
    protocol.sendRoomList(rooms);
}

void LobbyWorker::handleCreateRoom() {
    std::string roomName = protocol.receiveRoomName();
    if (lobby.createGameRoom(roomName, clientId, senderQueue)) {
        protocol.sendMsg({ActionCode::ROOM_CREATED});
    } else {
        protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
    }
}

void LobbyWorker::handleJoinRoom() {
    std::string roomName = protocol.receiveRoomName();
    if (lobby.joinGameRoom(roomName, clientId, senderQueue)) {
        protocol.sendMsg({ActionCode::JOIN_OK});
    } else {
        protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
    }
}

void LobbyWorker::handleStartGame() {
    if (lobby.startGameByClientId(clientId)) {
        if (onStartGame) onStartGame();
        stop();
    } else {
        protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
    }
}

void LobbyWorker::handleChooseCar() {
    std::string carType = protocol.receiveRoomName();
    CarConfig car{};
    car.carType = carType;
    if (lobby.chooseCarByClientId(clientId, car)) {
        protocol.sendMsg({ActionCode::CHOOSE_CAR_OK});
    } else {
        protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
    }
}

void LobbyWorker::handleSendUsername() {
    std::string username = protocol.receiveRoomName();
    lobby.setUsername(clientId, username);
}

void LobbyWorker::handleListPlayers() {
    auto players = lobby.getPlayersInRoomByClient(clientId);
    protocol.sendRoomList(players);
}

void LobbyWorker::handleListState() {
    bool started = lobby.isGameStartedByClient(clientId);
    protocol.sendRoomList({ started ? std::string("started") : std::string("waiting") });
}
