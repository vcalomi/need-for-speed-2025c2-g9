#ifndef SERVER_INPUT_HANDLER_H
#define SERVER_INPUT_HANDLER_H

#include <string>

#include "../common_src/thread.h"

class ServerInputHandler: public Thread {
private:
    std::string line;

public:
    ServerInputHandler();
    void run() override;
    bool should_stop_server() const;
};

#endif
