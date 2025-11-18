#pragma once
#include <memory>

#include "../../common/Dto/dto.h"
#include "../events/event.h"

class DtoHandler {
public:
    virtual ~DtoHandler() = default;

    virtual std::shared_ptr<Event> Handle(const std::shared_ptr<Dto>& dto) = 0;
};
