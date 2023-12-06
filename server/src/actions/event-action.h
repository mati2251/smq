#pragma once

#include <iostream>
#include <sys/epoll.h>

class EventAction
{
public:
    const int fd, efd;
    EventAction(int fd, int efd) : fd(fd), efd(efd) {}
    virtual void action() = 0;
    virtual epoll_event &getEpollEvent() = 0;

protected:
    epoll_event ev = {};
};
