#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include "../common_src/thread.h"
#include "../common_src/socket.h"
#include "client_handler.h"
#include "server_protocol.h"
#include "client_receiver.h"
#include "client_sender.h"
#include "client_monitor.h"

// es un thread para single-thread
class ClientHandler : public Thread {
private:
    Socket peer;
    Queue<ActionCode>& gameLoopQueue;
    ClientMonitor& clientMonitor;
    // std::unique_ptr<ClientReceiver>receiver;
    // std::unique_ptr<ClientSender> sender;
    bool connected;
    ServerProtocol protocol;
    Queue<std::vector<uint8_t>> sendQueue;

public:
    ClientHandler(Socket socket, ClientMonitor& monitor, Queue<ActionCode>& queue);
    // void run();
    void run() override;
    // void stop();
    void handleInput();   // Leer del cliente
    void handleOutput();  // Enviar al cliente
    bool isConnected();
    void stop() override;
    ~ClientHandler();
};

#endif