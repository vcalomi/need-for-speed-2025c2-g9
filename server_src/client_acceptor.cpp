#include "client_acceptor.h"

#include <algorithm>
#include <utility>

#include <sys/socket.h>

#include "client_handler.h"

ClientAcceptor::ClientAcceptor(const std::string& port, ClientMonitor& monitor,
                               Queue<ClientCommand>& queue):
        clientMonitor(monitor), gameLoopQueue(queue), acceptor(port.c_str()), nextClientId(0) {}

void ClientAcceptor::run() {
    while (should_keep_running()) {
        try {
            Socket socket = acceptor.accept();
            if (!should_keep_running()) {
                break;
            }
            int clientId = nextClientId++;
            ClientHandler* client =
                    new ClientHandler(std::move(socket), clientMonitor, gameLoopQueue, clientId);
            reap();
            clients.push_back(client);
            client->start();
        } catch (const LibError& e) {
            this->stop();
        } catch (const std::exception& e) {
            this->stop();
        }
    }
    clear();
}

void ClientAcceptor::reap() {
    auto new_end = std::remove_if(clients.begin(), clients.end(), [](ClientHandler* c) {
        bool is_dead = !c->is_alive();
        if (is_dead) {
            c->join();
            delete c;
        }
        return is_dead;
    });
    clients.erase(new_end, clients.end());
}

void ClientAcceptor::clear() {
    for (auto& client: clients) {
        client->stop();
    }
    for (auto& client: clients) {
        client->join();
        delete client;
    }
    clients.clear();
}

void ClientAcceptor::close() {
    this->stop();
    acceptor.shutdown(SHUT_RDWR);
    acceptor.close();
}

ClientAcceptor::~ClientAcceptor() {}
