#include "response-handler.h"

void ResponseHandler::handle(request req)
{
    response res = {.code = response_code::SUCCESS, .request_id = req.id, .message = "OK"};
    this->handle(res, req.from);
}

void ResponseHandler::handle(request req, std::exception &e)
{
    response res;
    res.code = response_code::UNKNOWN_ERROR;
    res.request_id = req.id;
    res.message = e.what();
    try
    {
        throw e;
    }
    catch (InvalidRequestException &_)
    {
        res.code = response_code::INVALID_REQUEST;
    }
    catch (ActionUnknownException &_)
    {
        res.code = response_code::ACTION_UNKNOWN;
    }
    catch (ClientAlreadySubscriberException &_)
    {
        res.code = response_code::CLIENT_ALREADY_SUBSCRIBER;
    }
    catch (ClientAlreadyPublisherException &_)
    {
        res.code = response_code::CLIENT_ALREADY_PUBLISHER;
    }
    catch (TopicNotFoundException &_)
    {
        res.code = response_code::TOPIC_NOT_FOUND;
    }
    catch (ClientNotSubscriberException &_)
    {
        res.code = response_code::CLIENT_NOT_SUBSCRIBER;
    }
    catch (ClientNotPublisherException &_)
    {
        res.code = response_code::CLIENT_NOT_PUBLISHER;
    }
    catch (std::exception &_)
    {
        res.code = response_code::UNKNOWN_ERROR;
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
