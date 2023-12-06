#pragma once
#define BUFFER_SIZE 1024
#define TOPIC_SIZE 100
#define MESSAGE_SIZE_KB 100

#include <csignal>
#include "../event-action.h"
#include "../../structs/request.h"
#include "invalid-request-exception.hpp"


class ClientReadAction : public EventAction
{
    public:
    ClientReadAction(int fd, int efd);
    void action();
private:
    int epoll_fd;
    request readRequest();
    request getType(std::string buffer_str= "");
    bool checkEndOfRequest(std::string partRequest, int &bracetCount);
};