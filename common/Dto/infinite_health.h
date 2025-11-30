#ifndef INFINITE_HEALTH_DTO_H
#define INFINITE_HEALTH_DTO_H
#include <string>

#include "../common_codes.h"

#include "dto.h"

class InfiniteHealthDto: public Dto {
public:
    std::string username;
    InfiniteHealthDto(const std::string& username);
};

#endif
