#pragma once

#include <iostream>
#include "../../structs/request.h"
#include "../handler.h"
#include "../../server-state/server-state.h"
#include "../response-handler/response-handler.h"
#include "../message-handler/message-handler.h"
#include "../action-handler/action-handler.h"

class RequestHandler final : public Handler {
public:
    static RequestHandler &getInstance();

    void handle(const std::string &req_str, int from);

    void handle(request req) override;

    void setEfd(int efd) override;

private:
    int efd;
    static RequestHandler *instance;
    MessageHandler *messageHandler;
    ActionHandler *actionHandler;
    ResponseHandler *responseHandler;

    RequestHandler();

    ~RequestHandler() override = default;

    RequestHandler(const RequestHandler &) = delete;

    RequestHandler &operator=(const RequestHandler &) = delete;
};
