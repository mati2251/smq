#pragma once

#include <sys/epoll.h>

class EventAction
{
public:
    const int fd, efd;
    EventAction(const int fd, const int efd) : fd(fd), efd(efd) {}
    virtual ~EventAction() = default;
    virtual void action() = 0;
    epoll_event &getEpollEvent(){
        return this->ev;
    };
protected:
    epoll_event ev = {};
};
