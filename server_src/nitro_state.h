#include <chrono>
#ifndef NITRO_STATE_H
#define NITRO_STATE_H

struct NitroState {
    int clientId; 
    std::chrono::steady_clock::time_point activationTime;
    bool isActive;

    NitroState() : clientId(-1), isActive(false) {}
    NitroState(int id) : clientId(id), isActive(false) {}
};

#endif