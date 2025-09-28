#ifndef SERVER_INPUT_HANDLER_H
#define SERVER_INPUT_HANDLER_H

#include "../common_src/thread.h"
#include <string>

class ServerInputHandler : public Thread {
private:
    std::string line;

public:
    explicit ServerInputHandler();
    void run() override;
    bool should_stop_server() const;
};

#endif