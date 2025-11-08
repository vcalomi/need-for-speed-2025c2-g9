#include "dto.h"

Dto::Dto() {}

Dto::Dto(uint8_t code): code(code), username("") {}

Dto::Dto(uint8_t code, const std::string& username): code(code), username(username) {}

Dto::~Dto() {}

uint8_t Dto::return_code() const { return code; }

std::string Dto::get_username() { return username; }
