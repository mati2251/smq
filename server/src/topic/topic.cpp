#include "topic.h"

Topic::Topic(std::string name)
{
    this->name = name;
}

void Topic::addSubscriber(ClientWriteAction *client)
{
    for (auto c : this->subscribers)
    {
        if (c->orginal_fd == client->orginal_fd)
        {
            return;
        }
    }
    std::cout << "Client " << client->orginal_fd << " subscribed to " << this->name << std::endl;
    this->subscribers.insert(client);
}

void Topic::addPublisher(int fd)
{
    for (auto client : this->subscribers)
    {
        if (client->orginal_fd == fd)
        {
            return;
        }
    }
    this->publishers.insert(fd);
    std::cout << "Client " << fd << " register as publisher to " << this->name << std::endl;
}

void Topic::removeSubscriber(int fd)
{
    for (auto client : this->subscribers)
    {
        if (client->orginal_fd == fd)
        {
            this->subscribers.erase(client);
            std::cout << "Client " << fd << " unsubscribed from " << this->name << std::endl;
            return;
        }
    }
}

void Topic::removePublisher(int fd)
{
    int erased = this->publishers.erase(fd);
    if (erased != 0)
    {
        std::cout << "Client " << fd << " unregister as unpublisher from " << this->name << std::endl;
    }
}

void Topic::publish(message msg)
{
    for (auto client : this->subscribers)
    {
        client->addMessage(msg);
        client->addToEpollIfNotExists();
    }
}

std::string Topic::getName()
{
    return this->name;
}
