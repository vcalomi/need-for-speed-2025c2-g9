#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include "../common_src/thread.h"
#include "../common_src/socket.h"
#include "client_handler.h"
#include "server_protocol.h"
#include "client_receiver.h"
#include "client_sender.h"
#include "client_monitor.h"

class ClientHandler {
private:
    Socket peer;
    Queue<ActionCode>& gameLoopQueue;
    ClientMonitor& clientMonitor;
    Queue<std::vector<uint8_t>> clientQueue;
    ServerProtocol protocol;
    ClientReceiver receiver;
    ClientSender sender;
    std::atomic_bool keep_running;

public:
    ClientHandler(Socket socket, ClientMonitor& monitor, Queue<ActionCode>& queue);
    void start();
    bool isConnected();
    void join();
    void stop();
    bool is_alive() const;
    ~ClientHandler();
};

#endif