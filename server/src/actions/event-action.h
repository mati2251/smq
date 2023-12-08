#pragma once

#include <iostream>
#include <sys/epoll.h>

class EventAction
{
public:
    const int fd, efd;
    EventAction(int fd, int efd) : fd(fd), efd(efd) {}
    virtual void action() = 0;
    epoll_event &getEpollEvent(){
        return this->ev;
    };
protected:
    epoll_event ev = {};
};
