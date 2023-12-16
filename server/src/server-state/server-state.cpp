#include "server-state.h"

ServerState* ServerState::instance = nullptr;


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

Topic *ServerState::getTopic(std::string name)
{
    for (auto topic : this->topics)
    {
        if (topic->getName() == name)
        {
            return topic;
        }
    }
    return nullptr;
}
