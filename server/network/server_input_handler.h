#ifndef SERVER_INPUT_HANDLER_H
#define SERVER_INPUT_HANDLER_H

#include <string>
#include <functional>

#include "../../common/thread.h"

class ServerInputHandler: public Thread {
private:
    std::string line;
    std::function<void()> shutdownCallback;

public:
    ServerInputHandler();
    void run() override;
    void setShutdownCallback(std::function<void()> callback);
};

#endif
