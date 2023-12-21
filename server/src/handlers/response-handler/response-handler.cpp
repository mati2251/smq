#include "response-handler.h"

void ResponseHandler::handle(request req)
{
    response res = { .code = response_code::SUCCESS, .message = "" };
    this->handle(res, req.from);
}

void ResponseHandler::handle(request req, std::exception &e)
{
    response res;
    try {
        throw e;
    }
    catch (InvalidRequestException &_){
        res = { .code = response_code::INVALID_REQUEST, .message = e.what() };
    }
    catch (ActionUnknownException &_){
        res = { .code = response_code::ACTION_UNKNOWN, .message = e.what() };
    }
    catch (DeserializationException &_){
        res = { .code = response_code::INVALID_REQUEST, .message = e.what() };
    }
    catch (ClientAlreadySubscriberException &_){
        res = { .code = response_code::CLIENT_ALREADY_SUBSCRIBER, .message = e.what() };
    }
    catch (ClientAlreadyPublisherException &_){
        res = { .code = response_code::CLIENT_ALREADY_PUBLISHER, .message = e.what() };
    }
    catch (TopicNotFoundException &_){
        res = { .code = response_code::TOPIC_NOT_FOUND, .message = e.what() };
    }
    catch (ClientNotSubscriberException &_){
        res = { .code = response_code::CLIENT_NOT_SUBSCRIBER, .message = e.what() };
    }
    catch (ClientNotPublisherException &_){
        res = { .code = response_code::CLIENT_NOT_PUBLISHER, .message = e.what() };
    }
    catch (std::exception &_){
        res = { .code = response_code::UNKNOWN_ERROR, .message = e.what() };
    }
    this->handle(res, req.from);
}

void ResponseHandler::handle(response res, int fd)
{
    for (auto client : ServerState::getInstance().clients)
    {
        if (client->orginal_fd == fd)
        {
            client->addResponse(res);
        }
    }
}

void ResponseHandler::setEfd(int efd)
{
    this->efd = efd;
}
