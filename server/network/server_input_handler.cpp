#include "server_input_handler.h"

#include <iostream>

ServerInputHandler::ServerInputHandler(): line("") {}

void ServerInputHandler::run() {
    try {
        while (should_keep_running()) {
            std::getline(std::cin, line);
            if (line == "q") {
                this->stop();
                break;
            }
        }
    } catch (const std::exception& e) {
        this->stop();
    }
}
