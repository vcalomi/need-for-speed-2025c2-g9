#include "send_checkpoints_handler.h"

#include <iostream>

std::shared_ptr<Event> SendCheckpointsHandler::Handle(const std::shared_ptr<Dto>& dto) {
    auto d = std::dynamic_pointer_cast<CheckpointDto>(dto);
    if (!d)
        return nullptr;
    return std::make_shared<CheckPointEvent>(d->id, d->x, d->y);
}
