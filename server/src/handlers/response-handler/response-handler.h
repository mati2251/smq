#pragma once
#include "../handler.h"
#include "../../structs/request.h"
#include "../../structs/response.h"
#include "../../actions/client-read-action/invalid-request-exception.hpp"
#include "../../handlers/action-handler/action-exception.hpp"
#include "../../topic/topic-exceptions.hpp"
#include "../../server-state/server-state.h"

class ResponseHandler : public Handler
{
public:
    ResponseHandler() = default;
    ~ResponseHandler() = default;
    void handle(request req) override;
    void handle(request req, std::exception &e);
    void handle(response res, int fd);
    void setEfd(int efd) override;
};