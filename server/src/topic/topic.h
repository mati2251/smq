#pragma once
#include <unordered_set>
#include <string>
#include "../actions/client-write-action/client-write-action.h"

class Topic
{
public:
    Topic(std::string name);
    void addSubscriber(ClientWriteAction *client);
    void addPublisher(int fd);
    void removeSubscriber(int fd);
    void removePublisher(int fd);
    void publish(message msg);
    std::string getName();
private:
    std::string name;
    std::unordered_set<ClientWriteAction *> subscribers = {};
    std::unordered_set<int> publishers = {};
};