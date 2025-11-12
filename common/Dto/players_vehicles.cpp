#include "players_vehicles.h"

#include "../common_codes.h"

PlayersVehiclesDto::PlayersVehiclesDto(int vehicle1_id, int vehicle2_id):
        Dto(uint8_t(ActionCode::SEND_PLAYERS_VEHICLES)), vehicle1_id(vehicle1_id), vehicle2_id(vehicle2_id) {}
