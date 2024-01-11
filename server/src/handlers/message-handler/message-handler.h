#pragma once

#include "../../structs/request.h"
#include "../handler.h"

#include "../../server-state/server-state.h"


class MessageHandler final : public Handler
{
public:
    void handle(request req) override;
    void setEfd(int efd) override;
};
