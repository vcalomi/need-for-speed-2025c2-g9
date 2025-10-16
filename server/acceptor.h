#pragma once 

#include "../common/thread.h"
#include "../common/socket.h"

class Acceptor : public Thread {
    
    private:
        
        Socket listen_socket_;
    public:
        Acceptor(const char* port);
        ~Acceptor();
        void run() override;
        void stop() override;
}