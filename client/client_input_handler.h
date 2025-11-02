#ifndef CLIENT_INPUT_HANDLER_H
#define CLIENT_INPUT_HANDLER_H

#include <functional>
#include <string>
#include <utility>

#include "../common/thread.h"

class ClientInputHandler: public Thread {
private:
    std::function<void()> onQuit;
    std::string line;

public:
    explicit ClientInputHandler(std::function<void()> onQuitCb):
            onQuit(std::move(onQuitCb)), line("") {}

    void run() override {
        try {
            while (should_keep_running()) {
                if (!std::getline(std::cin, line)) {
                    break;
                }
                if (line == "q") {
                    if (onQuit)
                        onQuit();
                    this->stop();
                    break;
                }
            }
        } catch (...) {
            this->stop();
        }
    }
};

#endif
