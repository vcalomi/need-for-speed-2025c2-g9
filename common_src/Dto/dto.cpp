#include "dto.h"

Dto::Dto() {}

Dto::Dto(uint8_t code) : code(code), id_cliente(0) {}

Dto::Dto(uint8_t code, uint8_t id) : code(code), id_cliente(id) {}

Dto::~Dto() {}

uint8_t Dto::return_code() { return code; }

uint8_t Dto::get_cliente_id() { return id_cliente; }