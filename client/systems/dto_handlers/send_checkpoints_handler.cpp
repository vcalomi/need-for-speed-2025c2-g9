#include "send_checkpoints_handler.h"

#include <iostream>

std::shared_ptr<Event> SendCheckpointsHandler::Handle(const std::shared_ptr<Dto>& dto) {
    auto d = std::dynamic_pointer_cast<CheckpointDto>(dto);
    if (!d)
        return nullptr;

    std::cout << "[DtoHandler] Checkpoint received: ID " << d->id << " pos(" << d->x << ", " << d->y
              << ")\n";

    return std::make_shared<CheckPointEvent>(d->id, d->x, d->y);
}
