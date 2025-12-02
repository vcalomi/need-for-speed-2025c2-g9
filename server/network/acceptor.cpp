#include "acceptor.h"

#include <algorithm>
#include <utility>

#include <sys/socket.h>

#include "lobby.h"

Acceptor::Acceptor(const std::string& port, GameMonitor& gameMonitor):
        acceptor(port.c_str()), gameMonitor(gameMonitor), nextClientId(0) {}

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

            Lobby* lobby = new Lobby(std::move(socket), gameMonitor, clientId);
            lobbies.push_back(lobby);
            lobby->start();
            reap();
        } catch (const LibError& e) {
            break;
        } catch (const std::exception& e) {
            break;
        }
    }
    clear();
}

void Acceptor::reap() {
    auto new_end = std::remove_if(lobbies.begin(), lobbies.end(), [](Lobby* l) {
        bool can_reap = !l->is_alive() && !l->isGameStarted();
        if (can_reap) {
            l->join();
            delete l;
        }
        return can_reap;
    });
    lobbies.erase(new_end, lobbies.end());
}

void Acceptor::clear() {
    for (auto& lobby: lobbies) {
        if (!lobby->isGameStarted()) {
            lobby->stop();
            lobby->join();
            delete lobby;
        }
    }
    lobbies.clear();
}

void Acceptor::close() {
    this->stop();
    acceptor.shutdown(SHUT_RDWR);
    acceptor.close();
}

Acceptor::~Acceptor() {}
