#include "client_handler.h"

#include <utility>

#include <sys/socket.h>

#include "../common/queue.h"
#include "../common/socket.h"
#include "../common/thread.h"
#include "receiver.h"

ClientHandler::ClientHandler(Socket socket, GameLobby& lobby, int clientId):
        peer(std::move(socket)),
        gameLobby(lobby),
        protocol(peer),
        keep_running(true),
        senderQueue(),
        sender(protocol, senderQueue),
        state(ClientState::IN_LOBBY),
        clientId(clientId),
        lobbyWorker(
        protocol,
        gameLobby,
        clientId,
        senderQueue,
        [this]{
            startThreads();
        }
        
    ) {
        gameLobby.registerStartNotifier(clientId, [this] {
        std::cout << "ClientHandler: Starting game for client " << this->clientId << " via StartNotifier" << std::endl;
        startThreads();
        });
    }

void ClientHandler::startThreads() {
    std::cout << "ClientHandler: Starting game for client " << this->clientId << std::endl;
    Queue<std::shared_ptr<Dto>>& gameQueue = gameLobby.getGameQueueForClient(this->clientId);
    receiver = std::make_unique<Receiver>(protocol, gameQueue);
    receiver->start();
    sender.start();
    state = ClientState::IN_GAME;
}

void ClientHandler::start() {
    keep_running = true;
    lobbyWorker.start();
}

void ClientHandler::join() {
    if (lobbyWorker.is_alive()) lobbyWorker.join();
    if (receiver) receiver->join();
    if (sender.is_alive()) sender.join();
}

void ClientHandler::stop() {
    keep_running = false;
    if (lobbyWorker.is_alive()) lobbyWorker.stop();
    try {
        peer.close();
    } catch (const SocketClosed& e) {}
    senderQueue.close();
    if (receiver) receiver->stop();
    sender.stop();
}

bool ClientHandler::is_alive() const {
    bool recvAlive = receiver ? receiver->is_alive() : false;
    return lobbyWorker.is_alive() || recvAlive || sender.is_alive();
}

ClientHandler::~ClientHandler() { }
