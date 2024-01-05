#include "request-handler.h"

RequestHandler *RequestHandler::instance = nullptr;

RequestHandler::RequestHandler()
{
    messageHandler = new MessageHandler();
    actionHandler = new ActionHandler();
    responseHandler = new ResponseHandler();
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
    case request_type::INVALID:
        throw InvalidRequestException();
    }
    this->responseHandler->handle(req);
}

void RequestHandler::handle(std::string req_str, int from)
{
    request req;
    req.from = from;
    req.id = -1;
    try
    {
        req = deserializeRequest(req_str);
        req.from = from;
        this->handle(req);
    }
    catch (std::exception &e)
    {
        this->responseHandler->handle(req, e);
    }
}

void RequestHandler::setEfd(int efd)
{
    this->efd = efd;
    this->messageHandler->setEfd(efd);
    this->actionHandler->setEfd(efd);
}