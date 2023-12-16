#pragma once

#include <vector>
#include "../topic/topic.h"
#include "../actions/client-write-action/client-write-action.h"

class ServerState
{
public:
    int efd;
    static ServerState &getInstance();
    void addClient(ClientWriteAction *client);
    Topic *getTopic(std::string name);
private:
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