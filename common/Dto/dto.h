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
    uint8_t id_cliente;

public:
    Dto();
    explicit Dto(uint8_t code);
    explicit Dto(uint8_t code, uint8_t id);
    uint8_t get_cliente_id();
    virtual ~Dto();
    uint8_t return_code() const;
};
#endif
