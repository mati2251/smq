#include "request-handler.h"


RequestHandler* RequestHandler::instance = nullptr;

RequestHandler &RequestHandler::getInstance()
{
    if (instance == nullptr)
    {
        instance = new RequestHandler();
    }
    return *instance;
}

void RequestHandler::handleRequest(request req)
{
    switch (req.type)
    {
    case request_type::ACTION:
        std::cout<<"action"<<std::endl;
        break;
    case request_type::MESSAGE:
        std::cout<<"message"<<std::endl;
        break;
    default:
        std::cout<<"unknown"<<std::endl;
        break;
    }
}
