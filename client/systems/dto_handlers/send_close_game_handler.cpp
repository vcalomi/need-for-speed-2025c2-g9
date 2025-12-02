#include "send_close_game_handler.h"

#include "../../common/Dto/close_game.h"
#include "../events/close_game_event.h"

std::shared_ptr<Event> SendCloseGameHandler::Handle(const std::shared_ptr<Dto>& dto) {
    auto closeGameDto = std::dynamic_pointer_cast<CloseGameDto>(dto);
    if (!closeGameDto) {
        return nullptr;
    }
    return std::make_shared<CloseGameEvent>();
}