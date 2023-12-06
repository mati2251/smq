#pragma once

#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include "../event-action.h"
#include "../client-read-action/client-read-action.h"


class AcceptAction : public EventAction
{
public:
    AcceptAction(int fd, int efd);
    void action();
    epoll_event& getEpollEvent();
private:
};
