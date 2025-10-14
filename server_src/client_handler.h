#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <vector>

#include "../common_src/client_command.h"
#include "../common_src/nitro_message.h"
#include "../common_src/socket.h"
#include "../common_src/socket_closed.h"
#include "../common_src/thread.h"

#include "client_handler.h"
#include "client_monitor.h"
#include "client_receiver.h"
#include "client_sender.h"
#include "server_protocol.h"

class ClientHandler {
private:
    Socket peer;
    Queue<ClientCommand>& gameLoopQueue;
    ClientMonitor& clientMonitor;
    Queue<NitroMessage> clientQueue;
    ServerProtocol protocol;
    ClientReceiver receiver;
    ClientSender sender;
    std::atomic_bool keep_running;

public:
    ClientHandler(Socket socket, ClientMonitor& monitor, Queue<ClientCommand>& queue, int clientId);
    void start();
    void join();
    void stop();
    bool is_alive() const;
    ~ClientHandler();
};

#endif
