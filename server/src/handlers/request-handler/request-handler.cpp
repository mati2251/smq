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

void RequestHandler::handle(std::string req_str, int from)
{
    request req = deserializeRequest(req_str);
    req.from = from;
    this->handle(req);
}

void RequestHandler::handle(request req)
{
    try
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
    catch (std::exception &e)
    {
        this->responseHandler->handle(req, e);
    }
}

void RequestHandler::handle(response res, int fd)
{
    this->responseHandler->handle(res, fd);
}

void RequestHandler::setEfd(int efd)
{
    this->efd = efd;
    this->messageHandler->setEfd(efd);
    this->actionHandler->setEfd(efd);
}