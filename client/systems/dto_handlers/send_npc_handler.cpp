#include "send_npc_handler.h"

#include "../../common/Dto/npc.h"
#include "../events/new_npc_event.h"

std::shared_ptr<Event> SendNpcHandler::Handle(const std::shared_ptr<Dto>& dto) {
    auto d = std::dynamic_pointer_cast<NPCDto>(dto);
    if (!d)
        return nullptr;

    return std::make_shared<NewNpcEvent>(d->id, d->x, d->y);
}
