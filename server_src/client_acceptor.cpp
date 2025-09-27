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
    acceptor(port.c_str()) { }

void ClientAcceptor::run() {
    while (should_keep_running()) {
        try {
            Socket socket = acceptor.accept();
            ClientHandler* client = new ClientHandler(std::move(socket), clientMonitor, gameLoopQueue);
            // reap();
            clients.push_back(client);
            client->start();
        } catch(const std::exception& e) {
    
        }
        cleanupDisconnectedClients();
    }
    clear();
}

void ClientAcceptor::cleanupDisconnectedClients() {
    for (size_t i = 0; i < clients.size(); ) {
        if (!clients[i]->isConnected()) {
            delete clients[i];
            clients.erase(clients.begin() + i);
        } else {
            i++;
        }
    }
}

// void ClientAcceptor::reap() {
//     clients.remove_if([] (const std::unique_ptr<ClientHandler>& c) {
//     bool is_dead = !c->is_alive();
//         if (is_dead) {
//             c->join();
//         }
//         return is_dead;
//     });
// }

// void ClientAcceptor::clear() {
//     for (auto& client : clients) {
//         client->kill();
//         client->join();
//     }
//     clients.clear();
// }

void ClientAcceptor::stop() {
    Thread::stop();
    acceptor.shutdown(SHUT_RDWR);
}

void ClientAcceptor::clear() {
    for (auto client : clients) {
        // client->disconnect();
        delete client;
    }
    clients.clear();
}

ClientAcceptor::~ClientAcceptor() { stop(); }
