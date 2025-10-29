#include "client_handler.h"

#include <utility>

#include <sys/socket.h>

#include "../common_src/queue.h"
#include "../common_src/socket.h"
#include "../common_src/thread.h"
#include "receiver.h"

ClientHandler::ClientHandler(Socket socket, GameLobby& gameLobby, int clientId):
        peer(std::move(socket)),
        gameLobby(gameLobby),
        sender(protocol, senderQueue),
        protocol(peer),
        keep_running(true),
        state(),
        clientId(clientId) {}

void ClientHandler::start() {
    while (keep_running && ClientState::IN_LOBBY == state) {
        ActionCode action = protocol.receiveActionCode();
        handleLobbyCommand(action);
    }
}

void ClientHandler::join() {
    receiver->join();
    sender.join();
}

void ClientHandler::handleLobbyCommand(ActionCode action) {
    switch (action) {
        case ActionCode::LIST_ROOMS: {
            auto rooms = gameLobby.getAvailableRooms();
            protocol.sendRoomList(rooms);
            break;
        }
            
        case ActionCode::CREATE_ROOM: {
            std::string roomName = protocol.receiveRoomName(); // o creo ROOMCODE
            if (gameLobby.createGameRoom(roomName, clientId, senderQueue)) {
                protocol.sendMsg({ActionCode::ROOM_CREATED});
                state = ClientState::IN_GAME_ROOM;
            } else {
                protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
            }
            break;
        }
            
        case ActionCode::JOIN_ROOM: {
            std::string roomName = protocol.receiveRoomName();
            if (gameLobby.joinGameRoom(roomName, clientId, senderQueue)) {
                protocol.sendMsg({ActionCode::JOIN_OK});
                state = ClientState::IN_GAME_ROOM;
            } else {
                protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
            }
            break;
        }
            
        case ActionCode::START_GAME: {
            bool started = gameLobby.startGameByClientId(clientId);
            if (!started) {
                protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
            }
            break;
        }

        case ActionCode::CHOOSE_CAR: {
            CarConfig car = protocol.receiveCarConfig();
            if (gameLobby.chooseCarByClientId(clientId, car)) {
                protocol.sendMsg({ActionCode::CHOOSE_CAR_OK});
            } else {
                protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
            }
            break;
            }
            
        default:
            protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
            break;
    }
};

void ClientHandler::startGameThreads(Queue<ClientCommand>& gameQueue) {
    receiver = std::make_unique<Receiver>(protocol, gameQueue, clientId);
    receiver->start();
    sender.start();
    state = ClientState::IN_GAME;
}


void ClientHandler::stop() {
    keep_running = false;
    try {
        peer.close();
    } catch (const SocketClosed& e) {}
    senderQueue.close();
    receiver->stop();
    sender.stop();
}

bool ClientHandler::is_alive() const {
    return keep_running && receiver->is_alive() && sender.is_alive();
}

ClientHandler::~ClientHandler() { }
