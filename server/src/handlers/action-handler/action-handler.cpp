#include "action-handler.h"

void ActionHandler::handle(request req)
{
    action_topic action = deserializeAction(req.body);
    action.from = req.from;
    if (action.action == "SUBSCRIBE")
    {
        this->hanldeSubsribeAction(action);
    }
    else if (action.action == "UNSUBSCRIBE")
    {
        this->handleUnsubscribeAction(action);
    }
    else if (action.action == "PUBLISH")
    {
        this->handlePublishAction(action);
    }
    else if (action.action == "UNPUBLISH")
    {
        this->handleUnpublishAction(action);
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

void ActionHandler::hanldeSubsribeAction(action_topic act)
{
    Topic *t = ServerState::getInstance().addNewTopicIfNotExists(act.topic);
    auto client = getClientWriteAction(act.from);
    if (client == nullptr)
    {
        std::cerr << "During handle subsribe action client not found" << std::endl;
        return;
    }
    try
    {
        t->addSubscriber(client);
    }
    catch (const ClientAlreadySubscriberException &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void ActionHandler::handleUnsubscribeAction(action_topic act)
{
    try
    {
        Topic *t = ServerState::getInstance().getTopic(act.topic);
        t->removeSubscriber(act.from);
        if (t->isEmpty())
        {
            ServerState::getInstance().removeTopic(act.topic);
        }
    }
    catch (const TopicNotFoundException &e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (const ClientNotSubscriberException &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void ActionHandler::handlePublishAction(action_topic act)
{
    Topic *t = ServerState::getInstance().addNewTopicIfNotExists(act.topic);
    try
    {
        t->addPublisher(act.from);
    }
    catch (const ClientAlreadyPublisherException &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void ActionHandler::handleUnpublishAction(action_topic act)
{
    try
    {
        Topic *t = ServerState::getInstance().getTopic(act.topic);
        t->removePublisher(act.from);
        if (t->isEmpty())
        {
            ServerState::getInstance().removeTopic(act.topic);
        }
    }
    catch (const TopicNotFoundException &e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (const ClientNotPublisherException &e)
    {
        std::cerr << e.what() << std::endl;
    }
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