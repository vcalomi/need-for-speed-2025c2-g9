#include <iostream>
#include <QApplication>
#include "lobby/mainwindow.h"
#include "client.h"

#define NUMBER_ARG 3
#define MSG_ERROR_STDIN "Command line: <hostname> <port>"
#define ARG_INDEX_HOSTNAME 1
#define ARG_INDEX_PORT 2

int main(int argc, char* argv[]) {
    if (argc != NUMBER_ARG) {
        std::cout << MSG_ERROR_STDIN << std::endl;
        return 1;
    }
    std::string hostname = std::string(argv[ARG_INDEX_HOSTNAME]);
    std::string port = std::string(argv[ARG_INDEX_PORT]);
    
    try {
        QApplication app(argc, argv);
        ClientProtocol protocol(hostname, port);
        // 1. Ejecutar Lobby
        bool game_started = false;
        MainWindow lobby(protocol, std::ref(game_started));
        lobby.show();
        app.exec();

        // 2. Si el juego inició, ejecutar Client
        if (game_started) {
            std::cout << "Game started! Launching game client..." << std::endl;
            Client client(protocol);
            client.run();
        }

        return 0;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
}