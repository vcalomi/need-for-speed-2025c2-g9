#include "client_acceptor.h"
#include <utility>
#include <algorithm>
#include <sys/socket.h>
#include "client_handler.h"
/*
Cada vez que llega un cliente, crea un ClientHandler con su peer socket
Administra la vida de los ClientHandler (limpieza con reap() y clear())
*/

ClientAcceptor::ClientAcceptor(const std::string& port, ClientMonitor& monitor, Queue<ActionCode>& queue) :
    clientMonitor(monitor), 
    gameLoopQueue(queue),
    acceptor(port.c_str()) {}
    // keep_running(true) {}

void ClientAcceptor::run() {
    while (should_keep_running()) {
        try {
            Socket socket = acceptor.accept();
            if (!should_keep_running()) {
                break;  
            } 
            ClientHandler* client = new ClientHandler(std::move(socket), clientMonitor, gameLoopQueue);
            reap();
            clients.push_back(client);
            client->start();
        } catch (const LibError& e) {
            this->stop();
        } catch(const std::exception& e) {
            this->stop();
        }
    }
    clear();
}

void ClientAcceptor::reap() {
    auto new_end = std::remove_if(clients.begin(), clients.end(),
        [] (ClientHandler* c) {
            bool is_dead = !c->is_alive();
            if (is_dead) {
                c->join();
                delete c;
            }
            return is_dead;
        }
    );
    clients.erase(new_end, clients.end());
}

void ClientAcceptor::clear() {
    for (auto& client : clients) {
        client->stop();
    }
    for (auto& client : clients) {
        client->join();
        delete client;
    }
    clients.clear();
}

void ClientAcceptor::close() {
    // keep_running = false;
    this->stop();
    acceptor.shutdown(SHUT_RDWR);
    acceptor.close();
}

ClientAcceptor::~ClientAcceptor() {}
