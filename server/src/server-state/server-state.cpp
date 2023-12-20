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
    for (auto topic : this->topics)
    {
        topic->removePublisher(orginal_fd);
        topic->removeSubscriber(orginal_fd);
        if (topic->isEmpty())
        {
            this->removeTopic(topic->getName());
        }
    }
    for (auto it = this->clients.begin(); it != this->clients.end(); ++it)
    {
        if ((*it)->orginal_fd == orginal_fd)
        {
            this->clients.erase(it);
            break;
        }
    }
}

Topic *ServerState::getTopic(std::string name)
{
    std::lock_guard<std::mutex> lock(this->topics_mutex);
    for (auto topic : this->topics)
    {
        if (topic->getName() == name)
        {
            return topic;
        }
    }
    throw TopicNotFoundException(name);
}

Topic *ServerState::addNewTopicIfNotExists(std::string topic_name)
{
    try
    {
        return this->getTopic(topic_name);
    }
    catch (const TopicNotFoundException &e)
    {
        std::lock_guard<std::mutex> lock(this->topics_mutex);
        auto topic = new Topic(topic_name);
        ServerState::getInstance().topics.push_back(topic);
        std::cout << "New topic " << topic_name << " created" << std::endl;
        return topic;
    }
}

void ServerState::removeTopic(std::string name)
{
    std::lock_guard<std::mutex> lock(this->topics_mutex);
    for (auto it = this->topics.begin(); it != this->topics.end(); ++it)
    {
        if ((*it)->getName() == name)
        {
            this->topics.erase(it);
            std::cout << "Topic " << name << " removed" << std::endl;
            break;
        }
    }
}
