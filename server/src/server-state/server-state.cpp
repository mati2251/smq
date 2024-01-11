#include "server-state.h"
#include "../topic/topic-exceptions.hpp"

ServerState *ServerState::instance = nullptr;

ServerState &ServerState::getInstance()
{
    if (instance == nullptr)
    {
        instance = new ServerState();
    }
    return *instance;
}

void ServerState::addClient(ClientWriteAction *client)
{
    this->clients.push_back(client);
}

void ServerState::removeClient(const int orginal_fd)
{
    std::unique_lock lock(this->topics_mutex);
    for (const auto topic : this->topics)
    {
        try
        {
            topic->removePublisher(orginal_fd);
        }
        catch (std::exception &_)
        {
        }
        try
        {
            topic->removeSubscriber(orginal_fd);
        }
        catch (std::exception &_)
        {
        }
        if (topic->isEmpty())
        {
            this->removeTopicWithoutLock(topic->getName());
        }
    }
    lock.unlock();
    for (auto it = this->clients.begin(); it != this->clients.end(); ++it)
    {
        if ((*it)->orginal_fd == orginal_fd)
        {
            delete (*it);
            this->clients.erase(it);
            break;
        }
    }
}

Topic *ServerState::getTopicWithoutLock(const std::string &name) const {
    for (const auto topic : this->topics)
    {
        if (topic->getName() == name)
        {
            return topic;
        }
    }
    throw TopicNotFoundException(name);
}

Topic *ServerState::getTopic(const std::string &name)
{
    std::lock_guard lock(this->topics_mutex);
    return this->getTopicWithoutLock(name);
}

Topic *ServerState::addNewTopicIfNotExists(const std::string &topic_name)
{
    std::lock_guard lock(this->topics_mutex);
    try
    {
        return this->getTopicWithoutLock(topic_name);
    }
    catch (const TopicNotFoundException &_)
    {
        const auto topic = new Topic(topic_name);
        getInstance().topics.push_back(topic);
        std::cout << "New topic " << topic_name << " created" << std::endl;
        return topic;
    }
}

void ServerState::removeTopicWithoutLock(const std::string& name)
{
    if (const int erase_num = erase_if(this->topics, [name](const auto &t)
                                       { return t->getName() == name; }); erase_num != 0)
        std::cout << "Topic " << name << " removed" << std::endl;

}

void ServerState::removeTopic(const std::string &name)
{
    std::lock_guard lock(this->topics_mutex);
    this->removeTopicWithoutLock(name);
}
