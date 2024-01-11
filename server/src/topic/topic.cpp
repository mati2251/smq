#include "topic.h"

#include "topic-exceptions.hpp"
#include <algorithm>

Topic::Topic(const std::string &name) {
    this->name = name;
}

void Topic::addSubscriber(ClientWriteAction *client) {
    std::lock_guard lock(this->subscribers_mutex);
    if (const auto it = std::ranges::find_if(this->subscribers, [client](const auto &c) {
        return c->orginal_fd == client->orginal_fd;
    }); it != this->subscribers.end()) {
        throw ClientAlreadySubscriberException(client->orginal_fd, this->name);
    }
    std::cout << "Client " << client->orginal_fd << " subscribed to " << this->name << std::endl;
    this->subscribers.insert(client);
}

void Topic::addPublisher(const int fd) {
    std::lock_guard lock(this->publishers_mutex);
    !this->publishers.contains(fd)
        ? this->publishers.insert(fd)
        : throw ClientAlreadyPublisherException(fd, this->name);
    std::cout << "Client " << fd << " register as publisher to " << this->name << std::endl;
}

void Topic::removeSubscriber(int fd) {
    std::lock_guard lock(this->subscribers_mutex);
    if (const int erased = erase_if(this->subscribers, [fd](const auto &c) { return c->orginal_fd == fd; }); erased == 0)
        throw ClientNotSubscriberException(fd, this->name);
}

void Topic::removePublisher(const int fd) {
    std::lock_guard lock(this->publishers_mutex);
    if (const int erased = this->publishers.erase(fd); erased != 0) {
        std::cout << "Client " << fd << " unregister as unpublisher from " << this->name << std::endl;
    } else {
        throw ClientNotPublisherException(fd, this->name);
    }
}

void Topic::publish(const request &msg) {
    std::lock_guard lock(this->subscribers_mutex);
    if (this->subscribers.empty()) {
        throw NoSubribersException();
    }
    for (const auto client: this->subscribers) {
        client->addMessage(msg);
    }
}

bool Topic::checkIfPublisher(const int fd) {
    std::lock_guard lock(this->publishers_mutex);
    return this->publishers.contains(fd);
}

bool Topic::isEmpty() const {
    if (this->subscribers.empty() && !this->publishers.empty()) {
        return true;
    }
    return false;
}

std::string Topic::getName() {
    return this->name;
}
