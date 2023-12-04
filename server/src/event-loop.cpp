#include "event-loop.h"
#include <sys/epoll.h>
#include <iostream>
#include "actions/accept-action.h"

EventLoop::EventLoop(int &sock)
{
    this->sock = sock;
}

EventLoop::~EventLoop()
{
}

void EventLoop::Run()
{
    this->epoll_fd = epoll_create1(0);
    if (this->epoll_fd == -1)
    {
        std::cerr << "epoll_create1" << std::endl;
        exit(EXIT_FAILURE);
    }
    AcceptAction accept_action(this->sock, this->epoll_fd);
    epoll_event ev = accept_action.getEpollEvent();
    if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, this->sock, &ev) == -1)
    {
        std::cerr << "epoll_ctl: sock" << std::endl;
        exit(EXIT_FAILURE);
    }
    epoll_event events;
    while (true)
    {
        epoll_wait(this->epoll_fd, &events, 1, -1);
        EventAction* event_action = static_cast<EventAction*>(events.data.ptr);
        event_action->action();
    }
}

void EventLoop::Stop()
{
}
