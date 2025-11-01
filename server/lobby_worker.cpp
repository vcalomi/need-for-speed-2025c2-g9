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

void LobbyWorker::start() {
    alive = true;
    keep_running = true;
    th = std::thread(&LobbyWorker::run, this);
}

void LobbyWorker::stop() {
    keep_running = false;
}

void LobbyWorker::join() {
    if (th.joinable()) th.join();
}

bool LobbyWorker::is_alive() const {
    return (bool)alive;
}

void LobbyWorker::run() {
    try {
        while (keep_running) {
            ActionCode action = protocol.receiveActionCode();
            if (!dispatcher.dispatch(action)) {
                protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
            }
            // Si START_GAME fue recibido, el handler corta keep_running
        }
    } catch (const std::exception& e) {
        // opcional: logging
    }
    alive = false;
}

void LobbyWorker::initHandlers() {
    dispatcher.registerHandler(ActionCode::LIST_ROOMS, [this]{ handleListRooms(); });
    dispatcher.registerHandler(ActionCode::CREATE_ROOM, [this]{ handleCreateRoom(); });
    dispatcher.registerHandler(ActionCode::JOIN_ROOM, [this]{ handleJoinRoom(); });
    dispatcher.registerHandler(ActionCode::CHOOSE_CAR, [this]{ handleChooseCar(); });
    dispatcher.registerHandler(ActionCode::SEND_USERNAME, [this]{ handleSendUsername(); });
    dispatcher.registerHandler(ActionCode::START_GAME, [this]{ handleStartGame(); });
}

void LobbyWorker::handleListRooms() {
    auto rooms = lobby.getAvailableRooms();
    protocol.sendRoomList(rooms);
}

void LobbyWorker::handleCreateRoom() {
    std::string roomName = protocol.receiveString();
    if (lobby.createGameRoom(roomName, clientId, senderQueue)) {
        protocol.sendMsg({ActionCode::ROOM_CREATED});
    } else {
        protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
    }
}

void LobbyWorker::handleJoinRoom() {
    std::string roomName = protocol.receiveString();
    if (lobby.joinGameRoom(roomName, clientId, senderQueue)) {
        protocol.sendMsg({ActionCode::JOIN_OK});
    } else {
        protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
    }
}

void LobbyWorker::handleStartGame() {
    if (lobby.startGameByClientId(clientId)) {
        if (onStartGame) onStartGame();
        keep_running = false; // corta el loop de lobby
    } else {
        protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
    }
}

void LobbyWorker::handleChooseCar() {
    std::string carType = protocol.receiveString();
    CarConfig car{};
    car.carType = carType;
    if (lobby.chooseCarByClientId(clientId, car)) {
        protocol.sendMsg({ActionCode::CHOOSE_CAR_OK});
    } else {
        protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
    }
}

void LobbyWorker::handleSendUsername() {
    std::string username = protocol.receiveString();
    // TODO: persistir en lobby/room si se desea
    std::cout << "Cliente " << clientId << " username: " << username << std::endl;
}
