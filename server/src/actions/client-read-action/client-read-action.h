#pragma once
#define BUFFER_SIZE 1024
#define TOPIC_SIZE 100
#define MESSAGE_SIZE_KB 100

#include <csignal>
#include "../event-action.h"
#include "../../handlers/request-handler/request-handler.h"
#include "../../structs/request.h"
#include "../../request-util/request-util.h"
#include "invalid-request-exception.hpp"
#include "connection-close-exception.hpp"
#include <fcntl.h>


class ClientReadAction : public EventAction
{
    public:
    ClientReadAction(int fd, int efd);
    virtual ~ClientReadAction();
    void action();
private:
    std::string buffer;
    void readRequest();
    bool checkEndOfRequest();
    void closeConnection();
};