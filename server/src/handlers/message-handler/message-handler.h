#pragma once

#include "../../structs/request.h"
#include "../handler.h"
#include <iostream>
#include "../../structs/message.h"
#include "../../json/deserialization/deserialization.h"
#include "../../actions/client-write-action/client-write-action.h"

class MessageHandler : public Handler
{
public:
    void handle(request req);
    void setEfd(int efd);
};