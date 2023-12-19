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
    std::lock_guard<std::mutex> lock(this->topics_mutex);
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
    return nullptr;
}

Topic *ServerState::addNewTopicIfNotExists(std::string topic_name)
{
    std::lock_guard<std::mutex> lock(this->topics_mutex);
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
