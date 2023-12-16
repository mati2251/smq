#pragma once

#include <iostream>
#include "../../structs/request.h"
#include "../../topic/topic.h"
#include "../handler.h"
#include "../message-handler/message-handler.h"
#include "../action-handler/action-handler.h"
#include <vector>

class RequestHandler : public Handler
{
public:
    static RequestHandler &getInstance();
    void handle(request req);
    void setEfd(int efd);
private:
    int efd;
    static RequestHandler *instance;
    MessageHandler *messageHandler;
    ActionHandler *actionHandler;
    RequestHandler();
    ~RequestHandler() = default;
    RequestHandler(const RequestHandler &) = delete;
    RequestHandler &operator=(const RequestHandler &) = delete;
};