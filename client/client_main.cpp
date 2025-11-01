#include <iostream>
#include <QApplication>

#include "client.h"
#include "lobby/mainwindow.h"

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
        // 2. Iniciar la interfaz Qt
        QApplication app(argc, argv);
        MainWindow w;
        w.show();
        app.exec();

        // 1. Crear el cliente y conectar
        Client client(hostname, port);
        client.run();

        return 0;
    } catch (...) {

        return 1;
    }
}
