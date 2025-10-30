#include "acceptor.h"

#include <algorithm>
#include <utility>

#include <sys/socket.h>

Acceptor::Acceptor(const std::string& port, GameLobby& gameLobby):
        acceptor(port.c_str()), gameLobby(gameLobby), nextClientId(0) {}

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
            gameLobby.registerClient(std::move(socket), clientId);
            gameLobby.processClients();
        } catch (const LibError& e) {
            this->stop();
        } catch (const std::exception& e) {
            this->stop();
        }
    }
}

void Acceptor::close() {
    this->stop();
    acceptor.shutdown(SHUT_RDWR);
    acceptor.close();
}

Acceptor::~Acceptor() {}
