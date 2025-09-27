#include "client_handler.h"

#include "../common_src/thread.h"
#include "../common_src/socket.h"
#include "../common_src/queue.h"
#include "client_handler.h"
#include <sys/socket.h>

ClientHandler::ClientHandler(Socket socket, ClientMonitor& monitor, Queue<ActionCode>& queue)
    : peer(std::move(socket)), gameLoopQueue(queue), clientMonitor(monitor), connected(true), protocol(peer) {
        clientMonitor.addQueue(&sendQueue);
}

void ClientHandler::run() {
    while (should_keep_running() && connected) {
        try {
            // sender = std::make_unique<Sender>(peer);
            // receiver = std::make_unique<Receiver>(peer);
            handleInput();
            handleOutput();
        } catch(std::exception& e) {
            connected = false;
        }
    }
}

void ClientHandler::handleInput() {    
    if (peer.is_stream_recv_closed()) {
        connected = false;
        return;
    }
    ActionCode command = protocol.tryReceiveActionCode();
    gameLoopQueue.push(command);
}

void ClientHandler::handleOutput() {
    // Enviar mensajes pendientes al cliente
    std::vector<uint8_t> message;
    while (sendQueue.try_pop(message)) {
        try {
            protocol.sendMsg(message);
        } catch (...) {
            connected = false;
            break;
        }
    }
}

// void ClientHandler::stop() {
//     if (receiver) receiver->stop();
//     if (sender) sender->stop();

//     if (receiver) receiver->join();
//     if (sender) sender->join();
// }

bool ClientHandler::isConnected() {
    return connected && !peer.is_stream_recv_closed();
}


void ClientHandler::stop() {
    Thread::stop();
    connected = false;
}

ClientHandler::~ClientHandler() {
    clientMonitor.removeQueue(&sendQueue);
}
