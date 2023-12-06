#pragma once

#include <csignal>
#include "../event-action.h"

class ClientReadAction : public EventAction
{
    public:
    ClientReadAction(int fd, int efd);
    void action();
private:
    int epoll_fd;
};