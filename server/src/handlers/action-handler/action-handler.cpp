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
        throw ActionException();
    }
}

void ActionHandler::setEfd(int efd)
{
    this->efd = efd;
}

void ActionHandler::hanldeSubsribeAction(action_topic act)
{
    Topic *t = ServerState::getInstance().addNewTopicIfNotExists(act.topic);
    t->addSubscriber(getClientWriteAction(act.from));
}

void ActionHandler::handleUnsubscribeAction(action_topic act)
{
    Topic *t = ServerState::getInstance().getTopic(act.topic);
    // todo check if topic exists
    t->removeSubscriber(act.from);
    if (t->isEmpty())
    {
        ServerState::getInstance().removeTopic(act.topic);
    }
}

void ActionHandler::handlePublishAction(action_topic act)
{
    Topic *t = ServerState::getInstance().addNewTopicIfNotExists(act.topic);
    t->addPublisher(act.from);
    if (t->isEmpty())
    {
        ServerState::getInstance().removeTopic(act.topic);
    }
}

void ActionHandler::handleUnpublishAction(action_topic act)
{
    // todo check if topic exists
    Topic *t = ServerState::getInstance().getTopic(act.topic);
    t->removePublisher(act.from);
    
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