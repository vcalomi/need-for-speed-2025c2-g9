#include "send_player_game_finished_handler.h"

#include "../../common/Dto/player_game_finished.h"
#include "../events/player_game_finished_event.h"

std::shared_ptr<Event> SendPlayerGameFinishedHandler::Handle(const std::shared_ptr<Dto>& dto) {
    auto playerGameFinishedDto = std::dynamic_pointer_cast<PlayerGameFinishedDto>(dto);
    if (!playerGameFinishedDto) {
        return nullptr;
    }
    return std::make_shared<PlayerGameFinishedEvent>(
            dto->get_username(), playerGameFinishedDto->totalRaceTime,
            playerGameFinishedDto->totalPenalties, playerGameFinishedDto->finalPosition);
}