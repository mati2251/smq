#pragma once
#include "../structs/request.h"
#include <iostream>

class RequestHandler
{
public:
    static RequestHandler& getInstance();
    void handleRequest(request req);
private:
    static RequestHandler* instance;
    RequestHandler() = default;
    ~RequestHandler() = default;
    RequestHandler(const RequestHandler&) = delete;
    RequestHandler& operator=(const RequestHandler&) = delete;
};