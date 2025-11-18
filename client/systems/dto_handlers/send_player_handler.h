#pragma once
#include <memory>

#include "../../common/Dto/player.h"
#include "../../network/client.h"
#include "../events/player_joined_event.h"

#include "dto_handler.h"

class SendPlayerHandler: public DtoHandler {
public:
    explicit SendPlayerHandler(Client& client);

    std::shared_ptr<Event> Handle(const std::shared_ptr<Dto>& dto) override;

private:
    Client& client_;
};
