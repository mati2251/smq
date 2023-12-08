#pragma once

#include "../../structs/request.h"
#include "../handler.h"
#include <iostream>


class MessageHandler : public Handler
{
public:
    void handle(request req);
    void setEfd(int efd);
};