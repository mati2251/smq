#include "topic.h"

Topic::Topic(std::string name)
{
    this->name = name;
}

void Topic::addSubscriber(ClientWriteAction *client)
{
    std::lock_guard<std::mutex> lock(this->subscribers_mutex);
    auto it = std::find_if(this->subscribers.begin(), this->subscribers.end(), [client](const auto &c)
                           { return c->orginal_fd == client->orginal_fd; });
    if (it != this->subscribers.end())
    {
        throw ClientAlreadySubscriberException(client->orginal_fd, this->name);
    }
    std::cout << "Client " << client->orginal_fd << " subscribed to " << this->name << std::endl;
    this->subscribers.insert(client);
}

void Topic::addPublisher(int fd)
{
    std::lock_guard<std::mutex> lock(this->publishers_mutex);
    this->publishers.count(fd) == 0 ? this->publishers.insert(fd) : throw ClientAlreadyPublisherException(fd, this->name);  
    std::cout << "Client " << fd << " register as publisher to " << this->name << std::endl;
}

void Topic::removeSubscriber(int fd)
{
    std::lock_guard<std::mutex> lock(this->subscribers_mutex);
    int erased = erase_if(this->subscribers, [fd](const auto &c)
                          { return c->orginal_fd == fd; });
    if (erased == 0)
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

void Topic::publish(request msg)
{
    std::lock_guard<std::mutex> lock(this->subscribers_mutex);
    for (auto client : this->subscribers)
    {
        client->addMessage(msg);
    }
}

bool Topic::checkIfPublisher(int fd)
{
    std::lock_guard<std::mutex> lock(this->publishers_mutex);
    return this->publishers.count(fd) != 0;
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
