#ifndef DTO_H
#define DTO_H

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

const char VACIO[] = "";

class Dto {
private:
    uint8_t code;
    std::string username;

public:
    Dto();
    explicit Dto(uint8_t code);
    explicit Dto(uint8_t code, const std::string& username);
    std::string get_username();
    virtual ~Dto();
    uint8_t return_code() const;
};
#endif
