#include "server_input_handler.h"

#include <iostream>

ServerInputHandler::ServerInputHandler() : line("") {}

void ServerInputHandler::run() {
    try {
        while (should_keep_running()) {
            std::getline(std::cin, line);
            if (line == "q") {
                this->stop(); // Signal itself to stop
            }
        }
    } catch(const std::exception& e) {
        // Handle any exceptions during input
    }
}

bool ServerInputHandler::should_stop_server() const {
    return line == "q";
}
