#include "client_handler.h"

#include <utility>

#include <sys/socket.h>

#include "../../common/queue.h"
#include "../../common/socket.h"
#include "../../common/thread.h"

#include "receiver.h"

ClientHandler::ClientHandler(Socket socket, GameMonitor& gameMonitor, int clientId):
        peer(std::move(socket)),
        gameMonitor(gameMonitor),
        protocol(peer),
        keep_running(true),
        senderQueue(),
        state(ClientState::IN_LOBBY),
        clientId(clientId),
        gameStarted(false),
        lobby(protocol, gameMonitor, clientId, senderQueue,
              [this](std::shared_ptr<GameRoom> room) { startThreads(room); }

        ) {
    gameMonitor.registerStartNotifier(
            clientId, [this](std::shared_ptr<GameRoom> room) { startThreads(room); });
}

void ClientHandler::startThreads(std::shared_ptr<GameRoom> room) {
    if (gameStarted)
        return;

    try {
        if (lobby.is_alive()) {
            lobby.stop();
        }

        Queue<std::shared_ptr<Dto>>& gameQueue = room->getGameQueue();
        receiver = std::make_unique<Receiver>(protocol, gameQueue);
        sender = std::make_unique<Sender>(protocol, senderQueue);
        receiver->start();
        sender->start();
        state = ClientState::IN_GAME;
        gameStarted = true;
    } catch (const std::exception& e) {
        std::cout << "ClientHandler: ERROR starting game for client " << clientId << ": "
                  << e.what() << std::endl;
    }
}

void ClientHandler::start() {
    keep_running = true;
    lobby.start();
}

void ClientHandler::join() {
    if (sender)
        sender->join();
    if (receiver)
        receiver->join();
    lobby.join();
}

void ClientHandler::stop() {
    keep_running = false;
    try {
        peer.close();
    } catch (const SocketClosed& e) {}
    senderQueue.close();
    if (lobby.is_alive()) {
        lobby.stop();
    }
    if (receiver)
        receiver->stop();
    if (sender)
        sender->stop();
    try {
        gameMonitor.removeClient(clientId);
    } catch (...) {}
}

bool ClientHandler::is_alive() const {
    return lobby.is_alive() || (receiver && receiver->is_alive()) || (sender && sender->is_alive());
}

ClientHandler::~ClientHandler() {
    try {
        gameMonitor.removeClient(clientId);
    } catch (...) {}
}
#include <memory>
