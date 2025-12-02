#pragma once
#include <memory>

#include "../events/event.h"

#include "dto_handler.h"

class SendNpcHitHandler: public DtoHandler {
public:
    std::shared_ptr<Event> Handle(const std::shared_ptr<Dto>& dto) override;
};
