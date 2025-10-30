#ifndef DTO_H
#define DTO_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

const char VACIO[] = "";

class Dto
{
private:
    uint8_t code;
    uint8_t id_cliente;

public:
    Dto();
    explicit Dto(uint8_t code);
    explicit Dto(uint8_t code, uint8_t id);
    virtual ~Dto();
    uint8_t get_cliente_id();
    uint8_t return_code();
};
#endif
