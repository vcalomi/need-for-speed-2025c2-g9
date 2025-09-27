#include <chrono>
#ifndef NITRO_STATE_H
#define NITRO_STATE_H

struct NitroState {
    int ownerClientId; 
    std::chrono::steady_clock::time_point activationTime;
    bool isActive;
};

#endif