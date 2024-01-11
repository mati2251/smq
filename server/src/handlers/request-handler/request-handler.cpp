#include "request-handler.h"

#include "../message-handler/message-handler.h"
#include "../response-handler/response-handler.h"
#include "../../request-util/deserialization/deserialization.h"
#include "../action-handler/action-handler.h"
#include "../../actions/client-read-action/invalid-request-exception.hpp"

RequestHandler *RequestHandler::instance = nullptr;

RequestHandler::RequestHandler(): efd(0) {
    messageHandler = new MessageHandler();
    actionHandler = new ActionHandler();
    responseHandler = new ResponseHandler();
}

RequestHandler &RequestHandler::getInstance() {
    if (instance == nullptr) {
        instance = new RequestHandler();
    }
    return *instance;
}

void RequestHandler::handle(const request req) {
    switch (req.type) {
        case ACTION:
            this->actionHandler->handle(req);
            break;
        case MESSAGE:
            this->messageHandler->handle(req);
            break;
        default:
            throw InvalidRequestException();
    }
    this->responseHandler->handle(req);
}

void RequestHandler::handle(const std::string &req_str, const int from) {
    request req;
    req.from = from;
    req.id = -1;
    try {
        req = deserializeRequest(req_str);
        req.from = from;
        this->handle(req);
    } catch (std::exception &e) {
        this->responseHandler->handle(req, e);
    }
}

void RequestHandler::setEfd(const int efd) {
    this->efd = efd;
    this->messageHandler->setEfd(efd);
    this->actionHandler->setEfd(efd);
}
