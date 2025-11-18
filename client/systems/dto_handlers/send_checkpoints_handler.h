#pragma once
#include <memory>

#include "../../common/Dto/checkpoint.h"
#include "../events/checkpoint_event.h"

#include "dto_handler.h"

class SendCheckpointsHandler: public DtoHandler {
public:
    std::shared_ptr<Event> Handle(const std::shared_ptr<Dto>& dto) override;
};
