#include "topic.h"

Topic::Topic(std::string name)
{
    this->name = name;
}

void Topic::addSubscriber(ClientWriteAction *client)
{
    std::lock_guard<std::mutex> lock(this->subscribers_mutex);
    for (auto c : this->subscribers)
    {
        if (c->orginal_fd == client->orginal_fd)
        {
            throw ClientAlreadySubscriberException(client->orginal_fd, this->name);
        }
    }
    std::cout << "Client " << client->orginal_fd << " subscribed to " << this->name << std::endl;
    this->subscribers.insert(client);
}

void Topic::addPublisher(int fd)
{
    std::lock_guard<std::mutex> lock(this->publishers_mutex);
    for (auto client : this->subscribers)
    {
        if (client->orginal_fd == fd)
        {
            throw ClientAlreadyPublisherException(fd, this->name);
        }
    }
    this->publishers.insert(fd);
    std::cout << "Client " << fd << " register as publisher to " << this->name << std::endl;
}

void Topic::removeSubscriber(int fd)
{
    std::lock_guard<std::mutex> lock(this->subscribers_mutex);
    for (auto client : this->subscribers)
    {
        if (client->orginal_fd == fd)
        {
            this->subscribers.erase(client);
            std::cout << "Client " << fd << " unsubscribed from " << this->name << std::endl;
            return;
        }
    }
    throw ClientNotSubscriberException(fd, this->name);
}

void Topic::removePublisher(int fd)
{
    std::lock_guard<std::mutex> lock(this->publishers_mutex);
    int erased = this->publishers.erase(fd);
    if (erased != 0)
    {
        std::cout << "Client " << fd << " unregister as unpublisher from " << this->name << std::endl;
    }
    else
    {
        throw ClientNotPublisherException(fd, this->name);
    }
}

void Topic::publish(message msg)
{
    std::vector<ClientWriteAction *> clients_copy = {};
    {
        std::lock_guard<std::mutex> lock(this->subscribers_mutex);
        clients_copy = std::vector<ClientWriteAction *>(this->subscribers.begin(), this->subscribers.end());
    }
    for (auto client : this->subscribers)
    {
        client->addMessage(msg);
        client->addToEpollIfNotExists();
    }
}

bool Topic::checkIfPublisher(int fd)
{
    std::lock_guard<std::mutex> lock(this->publishers_mutex);
    for (auto client : this->publishers)
    {
        if (client == fd)
        {
            return true;
        }
    }
    return false;
}

bool Topic::isEmpty()
{
    if (this->subscribers.size() == 0 && this->publishers.size() == 0)
    {
        return true;
    }
    return false;
}

std::string Topic::getName()
{
    return this->name;
}
