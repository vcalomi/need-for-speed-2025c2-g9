#include "send_game_finished_handler.h"

#include "../../common/Dto/game_finished.h"
#include "../events/game_finished_event.h"

std::shared_ptr<Event> SendGameFinishedHandler::Handle(const std::shared_ptr<Dto>& dto) {
    auto gameFinishedDto = std::dynamic_pointer_cast<GameFinishedDto>(dto);
    if (!gameFinishedDto) {
        return nullptr;
    }
    return std::make_shared<GameFinishedEvent>();
}