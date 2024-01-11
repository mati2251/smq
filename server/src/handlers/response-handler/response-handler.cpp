#include "response-handler.h"
#include "../../actions/client-read-action/invalid-request-exception.hpp"
#include "../../topic/topic-exceptions.hpp"
#include "../../actions/client-write-action/full-buffer-exception.hpp"
#include "../../server-state/server-state.h"

void ResponseHandler::handle(const request req)
{
    const response res = {.code = SUCCESS, .request_id = req.id, .message = "OK"};
    this->handle(res, req.from);
}

void ResponseHandler::handle(const request &req, std::exception &e)
{
    response res;
    res.code = UNKNOWN_ERROR;
    res.request_id = req.id;
    res.message = e.what();
    try
    {
        throw e;
    }
    catch (InvalidRequestException &_)
    {
        res.code = INVALID_REQUEST;
    }
    catch (ActionUnknownException &_)
    {
        res.code = ACTION_UNKNOWN;
    }
    catch (ClientAlreadySubscriberException &_)
    {
        res.code = CLIENT_ALREADY_SUBSCRIBER;
    }
    catch (ClientAlreadyPublisherException &_)
    {
        res.code = CLIENT_ALREADY_PUBLISHER;
    }
    catch (TopicNotFoundException &_)
    {
        res.code = TOPIC_NOT_FOUND;
    }
    catch (ClientNotSubscriberException &_)
    {
        res.code = CLIENT_NOT_SUBSCRIBER;
    }
    catch (ClientNotPublisherException &_)
    {
        res.code = CLIENT_NOT_PUBLISHER;
    }
    catch (FullBufferException &_)
    {
        res.code = FULL_BUFFER;
    }
    catch (NoSubribersException &_) {
        res.code = NO_SUBSCRIBERS;
    }
    catch (std::exception &_)
    {
        res.code = UNKNOWN_ERROR;
    }
    this->handle(res, req.from);
}

void ResponseHandler::handle(const response &res, const int fd)
{
    for (const auto client : ServerState::getInstance().clients)
    {
        if (client->orginal_fd == fd)
        {
            client->addResponse(res);
        }
    }
}

void ResponseHandler::setEfd(const int efd)
{
    this->efd = efd;
}
