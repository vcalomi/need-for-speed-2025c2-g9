#include "send_player_handler.h"

SendPlayerHandler::SendPlayerHandler(Client& client): client_(client) {}

std::shared_ptr<Event> SendPlayerHandler::Handle(const std::shared_ptr<Dto>& dto) {
    auto d = std::dynamic_pointer_cast<PlayerDto>(dto);
    if (!d)
        return nullptr;
    return std::make_shared<PlayerJoinedEvent>(d->username, d->Type,
                                               d->username == client_.getUsername(), d->car_hp);
}
