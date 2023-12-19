#pragma once

#include <vector>
#include "../topic/topic.h"
#include "../actions/client-write-action/client-write-action.h"
#include <mutex>

class ServerState
{
public:
    int efd;
    static ServerState &getInstance();
    void addClient(ClientWriteAction *client);
    void removeClient(int orginal_fd);
    Topic *getTopic(std::string name);
    Topic *addNewTopicIfNotExists(std::string topic_name);
private:
    std::mutex topics_mutex;
    friend class MessageHandler;
    friend class ActionHandler;
    std::vector<Topic *> topics = {};
    std::vector<ClientWriteAction *> clients = {};
    static ServerState *instance;
    ServerState() = default;
    ~ServerState() = default;
    ServerState(const ServerState &) = delete;
    ServerState &operator=(const ServerState &) = delete;
};