#include "acceptor.h"

#include <algorithm>
#include <utility>

#include <sys/socket.h>

#include "client_handler.h"

Acceptor::Acceptor(const std::string& port):
        acceptor(port.c_str()), nextClientId(0) {}

void Acceptor::run() {
    while (should_keep_running()) {
        try {
            std::cout << "Acceptor: Esperando nueva conexión..." << std::endl;
            Socket socket = acceptor.accept();
            std::cout << "Acceptor: ¡Nueva conexión aceptada!" << std::endl;
            if (!should_keep_running()) {
                break;
            }
            int clientId = nextClientId++;
            ClientHandler* client =
                    new ClientHandler(std::move(socket), clientId);
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

void Acceptor::reap() {
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

void Acceptor::clear() {
    for (auto& client: clients) {
        client->stop();
    }
    for (auto& client: clients) {
        client->join();
        delete client;
    }
    clients.clear();
}

void Acceptor::close() {
    this->stop();
    acceptor.shutdown(SHUT_RDWR);
    acceptor.close();
}

Acceptor::~Acceptor() { clear(); }
