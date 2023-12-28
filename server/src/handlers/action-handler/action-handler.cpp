#include "action-handler.h"

void ActionHandler::handle(request req)
{
    std::string action_type = req.body.substr(0, req.body.find("\n\n"));
    if (action_type == "SUBSCRIBE")
    {
        this->hanldeSubsribeAction(req);
    }
    else if (action_type == "UNSUBSCRIBE")
    {
        this->handleUnsubscribeAction(req);
    }
    else if (action_type == "PUBLISH")
    {
        this->handlePublishAction(req);
    }
    else if (action_type == "UNPUBLISH")
    {
        this->handleUnpublishAction(req);
    }
    else
    {
        throw ActionUnknownException();
    }
}

void ActionHandler::setEfd(int efd)
{
    this->efd = efd;
}

void ActionHandler::hanldeSubsribeAction(request act)
{
    Topic *t = ServerState::getInstance().addNewTopicIfNotExists(act.topic);
    auto client = getClientWriteAction(act.from);
    if (client == nullptr)
    {
        std::cerr << "During handle subsribe action client not found" << std::endl;
        return;
    }
    // try
    // {
        t->addSubscriber(client);
    // }
//     catch (const ClientAlreadySubscriberException &e)
//     {
//         std::cerr << e.what() << std::endl;
//     }
}

void ActionHandler::handleUnsubscribeAction(request act)
{
    // try
    // {
        Topic *t = ServerState::getInstance().getTopic(act.topic);
        t->removeSubscriber(act.from);
        if (t->isEmpty())
        {
            ServerState::getInstance().removeTopic(act.topic);
        }
    // }
    // catch (const TopicNotFoundException &e)
    // {
        // std::cerr << e.what() << std::endl;
    // }
    // catch (const ClientNotSubscriberException &e)
    // {
        // std::cerr << e.what() << std::endl;
    // }
}

void ActionHandler::handlePublishAction(request act)
{
    Topic *t = ServerState::getInstance().addNewTopicIfNotExists(act.topic);
    // try
    // {
        t->addPublisher(act.from);
    // }
    // catch (const ClientAlreadyPublisherException &e)
    // {
        // std::cerr << e.what() << std::endl;
    // }
}

void ActionHandler::handleUnpublishAction(request act)
{
    // try
    // {
        Topic *t = ServerState::getInstance().getTopic(act.topic);
        t->removePublisher(act.from);
        if (t->isEmpty())
        {
            ServerState::getInstance().removeTopic(act.topic);
        }
    // }
    // catch (const TopicNotFoundException &e)
    // {
        // std::cerr << e.what() << std::endl;
    // }
    // catch (const ClientNotPublisherException &e)
    // {
    //     std::cerr << e.what() << std::endl;
    // }
}

ClientWriteAction *ActionHandler::getClientWriteAction(int client_id)
{
    for (auto client : ServerState::getInstance().clients)
    {
        if (client->orginal_fd == client_id)
        {
            return client;
        }
    }
    return nullptr;
}