#include "server-state.h"

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

void ServerState::removeClient(int orginal_fd)
{
    std::unique_lock<std::mutex> lock(this->topics_mutex);
    for (auto topic : this->topics)
    {
        try
        {
            topic->removePublisher(orginal_fd);
        }
        catch (std::exception &e)
        {
        }
        try
        {
            topic->removeSubscriber(orginal_fd);
        }
        catch (std::exception &e)
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

Topic *ServerState::getTopicWithoutLock(std::string name)
{
    for (auto topic : this->topics)
    {
        if (topic->getName() == name)
        {
            return topic;
        }
    }
    throw TopicNotFoundException(name);
}

Topic *ServerState::getTopic(std::string name)
{
    std::lock_guard<std::mutex> lock(this->topics_mutex);
    return this->getTopicWithoutLock(name);
}

Topic *ServerState::addNewTopicIfNotExists(std::string topic_name)
{
    std::lock_guard<std::mutex> lock(this->topics_mutex);
    try
    {
        return this->getTopicWithoutLock(topic_name);
    }
    catch (const TopicNotFoundException &e)
    {
        auto topic = new Topic(topic_name);
        ServerState::getInstance().topics.push_back(topic);
        std::cout << "New topic " << topic_name << " created" << std::endl;
        return topic;
    }
}

void ServerState::removeTopicWithoutLock(std::string name)
{
    int erase_num = erase_if(this->topics, [name](const auto &t)
                             { return t->getName() == name; });
    if (erase_num != 0)
        std::cout << "Topic " << name << " removed" << std::endl;
    
}

void ServerState::removeTopic(std::string name)
{
    std::lock_guard<std::mutex> lock(this->topics_mutex);
    this->removeTopicWithoutLock(name);
}
