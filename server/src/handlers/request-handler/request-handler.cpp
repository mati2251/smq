#include "request-handler.h"


RequestHandler* RequestHandler::instance = nullptr;

RequestHandler::RequestHandler()
{
    messageHandler = new MessageHandler();
    actionHandler = new ActionHandler();
}

RequestHandler &RequestHandler::getInstance()
{
    if (instance == nullptr)
    {
        instance = new RequestHandler();
    }
    return *instance;
}

void RequestHandler::handle(request req)
{
    switch (req.type)
    {
    case request_type::ACTION:
        this->actionHandler->handle(req);        
        break;
    case request_type::MESSAGE:
        this->messageHandler->handle(req);
        break;
    }
}

void RequestHandler::setEfd(int efd)
{
    this->efd = efd;
    this->messageHandler->setEfd(efd);
    this->actionHandler->setEfd(efd);
}
