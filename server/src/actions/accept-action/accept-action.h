#pragma once

#include "../event-action.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>

class AcceptAction : public EventAction
{
public:
    AcceptAction(int fd, int efd);
    void action();
    epoll_event& getEpollEvent();
private:
    int epoll_fd;
};
