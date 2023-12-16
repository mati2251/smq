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
    Topic *t = addNewTopicIfNotExists(act.topic);
    t->addSubscriber(getClientWriteAction(act.from));
}

void ActionHandler::handleUnsubscribeAction(action_topic act)
{
    Topic *t = addNewTopicIfNotExists(act.topic);
    t->removeSubscriber(act.from);
}

void ActionHandler::handlePublishAction(action_topic act)
{
    Topic *t = addNewTopicIfNotExists(act.topic);
    t->addPublisher(act.from);
}

void ActionHandler::handleUnpublishAction(action_topic act)
{
    Topic *t = addNewTopicIfNotExists(act.topic);
    t->removePublisher(act.from);
    std::cout << "Client " << act.from << " unregister as unpublisher from " << act.topic << std::endl;
}

Topic *ActionHandler::addNewTopicIfNotExists(std::string topic_name)
{
    for (auto topic : ServerState::getInstance().topics)
    {
        if (topic->getName() == topic_name)
        {
            return topic;
        }
    }
    auto topic = new Topic(topic_name);
    ServerState::getInstance().topics.push_back(topic);
    std::cout << "New topic " << topic_name << " created" << std::endl;
    return topic;
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