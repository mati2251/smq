#pragma once

#include <vector>
#include "../topic/topic.h"
#include "../actions/client-write-action/client-write-action.h"
#include <mutex>

class ServerState
{
public:
    int efd{};
    static ServerState &getInstance();
    void addClient(ClientWriteAction *client);
    void removeClient(int orginal_fd);
    Topic *getTopic(const std::string &name);
    Topic *addNewTopicIfNotExists(const std::string &topic_name);
    void removeTopic(const std::string &name);

private:
    std::mutex topics_mutex;
    friend class MessageHandler;
    friend class ActionHandler;
    friend class ResponseHandler;
    std::vector<Topic *> topics = {};
    std::vector<ClientWriteAction *> clients = {};
    static ServerState *instance;
    ServerState() = default;
    ~ServerState() = default;
    ServerState(const ServerState &) = delete;
    ServerState &operator=(const ServerState &) = delete;
    Topic *getTopicWithoutLock(const std::string &name) const;
    void removeTopicWithoutLock(const std::string& name);
};
