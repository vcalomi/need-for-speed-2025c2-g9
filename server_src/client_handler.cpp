#include "client_handler.h"

#include "../common_src/thread.h"
#include "../common_src/socket.h"
#include "../common_src/queue.h"
#include "client_handler.h"
#include <sys/socket.h>

ClientHandler::ClientHandler(Socket socket, ClientMonitor& monitor, Queue<ActionCode>& queue)
    : peer(std::move(socket)), 
    gameLoopQueue(queue), 
    clientMonitor(monitor),
    clientQueue(),
    protocol(peer),
    receiver(protocol, queue),
    sender(protocol, clientQueue),
    keep_running(true) {
        clientMonitor.addQueue(&clientQueue);
}

void ClientHandler::start() {
    sender.start();
    receiver.start();
}

void ClientHandler::join() {
    receiver.join();
    sender.join();
    std::cout << "receiver unido" << std::endl;
    std::cout << "sender unido" << std::endl;
}

void ClientHandler::stop() {
    keep_running = false;
    peer.shutdown(SHUT_RDWR);
    clientQueue.close();
    receiver.stop();
    sender.stop();
}

bool ClientHandler::isConnected() {
    return keep_running && !peer.is_stream_recv_closed();
}

bool ClientHandler::is_alive() const {
    return keep_running && receiver.is_alive() && sender.is_alive();
}

ClientHandler::~ClientHandler() { clientMonitor.removeQueue(&clientQueue); }
