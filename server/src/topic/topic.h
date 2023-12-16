#pragma once
#include <unordered_set>
#include <string>
#include "../actions/client-write-action/client-write-action.h"
#include <mutex>

class Topic
{
public:
    Topic(std::string name);
    void addSubscriber(ClientWriteAction *client);
    void addPublisher(int fd);
    void removeSubscriber(int fd);
    void removePublisher(int fd);
    void publish(message msg);
    bool checkIfPublisher(int fd);
    std::string getName();
private:
    std::mutex subscribers_mutex;
    std::mutex publishers_mutex;
    std::string name;
    std::unordered_set<ClientWriteAction *> subscribers = {};
    std::unordered_set<int> publishers = {};
};