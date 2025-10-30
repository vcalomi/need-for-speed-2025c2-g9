#include "client_handler.h"

#include <utility>

#include <sys/socket.h>

#include "../common_src/queue.h"
#include "../common_src/socket.h"
#include "../common_src/thread.h"
#include "receiver.h"

ClientHandler::ClientHandler(Socket socket, GameLobby& lobby, int clientId):
        peer(std::move(socket)),
        gameLobby(lobby),
        protocol(peer),
        keep_running(true),
        senderQueue(),
        sender(protocol, senderQueue),
        state(ClientState::IN_LOBBY),
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
            std::cout << "Enviadas " << rooms.size() << " salas disponibles al cliente " << clientId << std::endl;
            break;
        }
            
        case ActionCode::CREATE_ROOM: {
            std::string roomName = protocol.receiveRoomName(); // o creo ROOMCODE
            if (gameLobby.createGameRoom(roomName, clientId, senderQueue)) {
                std::cout << "Sala " << roomName << " creada por el cliente " << clientId << std::endl;
                protocol.sendMsg({ActionCode::ROOM_CREATED});
            } else {
                protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
            }
            break;
        }
            
        case ActionCode::JOIN_ROOM: {
            std::string roomName = protocol.receiveRoomName();
            if (gameLobby.joinGameRoom(roomName, clientId, senderQueue)) {
                protocol.sendMsg({ActionCode::JOIN_OK});
            } else {
                protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
            }
            break;
        }
            
        case ActionCode::START_GAME: {
            if (gameLobby.startGameByClientId(clientId)) {
                Queue<std::shared_ptr<Dto>>& gameQueue = gameLobby.getGameQueueForClient(clientId);
                receiver = std::make_unique<Receiver>(protocol, gameQueue);
                receiver->start();
                sender.start();
                state = ClientState::IN_GAME;
            } else {
                protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
            }
            break;
        }

        case ActionCode::CHOOSE_CAR: {
            // CarConfig car = protocol.receiveCarConfig();
            // if (gameLobby.chooseCarByClientId(clientId, car)) {
            //     protocol.sendMsg({ActionCode::CHOOSE_CAR_OK});
            // } else {
            //     protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
            // }
            break;
            }
            
        default:
            protocol.sendMsg({ActionCode::SEND_ERROR_MSG});
            break;
    }
};

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
