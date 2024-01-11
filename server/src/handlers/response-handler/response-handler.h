#pragma once
#include "../handler.h"
#include "../../structs/request.h"
#include "../../structs/response.h"
#include "../../handlers/action-handler/action-exception.hpp"


class ResponseHandler final : public Handler
{
public:
    ResponseHandler() = default;
    ~ResponseHandler() override = default;
    void handle(request req) override;
    void handle(const request &req, std::exception &e);
    void handle(const response &res, int fd);
    void setEfd(int efd) override;
};
