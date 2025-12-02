#include "send_npc_hit_handler.h"

#include <iostream>

#include "../../common/Dto/player_hit_npc.h"
#include "../events/npc_hit_event.h"

std::shared_ptr<Event> SendNpcHitHandler::Handle(const std::shared_ptr<Dto>& dto) {
    auto d = std::dynamic_pointer_cast<PlayerHitNPCDto>(dto);
    if (!d)
        return nullptr;

    std::cout << "[SendNpcHitHandler] Handling PlayerHitNPCDto for username: " << d->username
              << "\n";

    return std::make_shared<NpcHitEvent>(d->username);
}
