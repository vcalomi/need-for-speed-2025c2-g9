#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <vector>

#include "../common_src/socket.h"
#include "../common_src/socket_closed.h"
#include "../common_src/thread.h"
#include "../common_src/Dto/dto.h"
#include "../common_src/queue.h"

#include "receiver.h"
#include "sender.h"
#include "server_protocol.h"
#include "client_state.h"

class ClientHandler {
private:
    Socket peer;
    ServerProtocol protocol;
    Sender sender;
    std::unique_ptr<Receiver> receiver;
    std::atomic_bool keep_running;
    
public:
    Queue<Dto> senderQueue;    // para enviar al cliente
    ClientHandler(Socket socket);
    void start(Queue<Dto>& gameQueue);
    void join();
    void stop();
    bool is_alive() const;
    ~ClientHandler();
};

#endif
