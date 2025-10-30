#include "client_handler.h"

#include <utility>

#include <sys/socket.h>

#include "../common_src/queue.h"
#include "../common_src/socket.h"
#include "../common_src/thread.h"
#include "receiver.h"

ClientHandler::ClientHandler(Socket socket):
        peer(std::move(socket)),
        protocol(peer),
        senderQueue(),
        sender(protocol, senderQueue),
        keep_running(true){}

void ClientHandler::start(Queue<Dto>& gameQueue) {
    sender.start();
    receiver = std::make_unique<Receiver>(protocol, gameQueue);
    receiver->start();
}

void ClientHandler::join() {
    receiver->join();
    sender.join();
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
