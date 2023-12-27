#include "event-loop.h"

EventLoop::EventLoop(int &sock)
{
    this->sock = sock;
}

EventLoop::~EventLoop()
{
    close(this->sock);
}

void EventLoop::Run()
{
    this->epoll_fd = epoll_create1(0);
    RequestHandler::getInstance().setEfd(this->epoll_fd);
    if (this->epoll_fd == -1)
    {
        std::cerr << "Error epoll_create: " << std::strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
    AcceptAction accept_action(this->sock, this->epoll_fd);
    epoll_event ev = accept_action.getEpollEvent();
    if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, this->sock, &ev) == -1)
    {
        std::cerr << "Error epoll_ctl_add: " << std::strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
    epoll_event events;
    while (true)
    {
        epoll_wait(this->epoll_fd, &events, 1, -1);
        EventAction *event_action = static_cast<EventAction *>(events.data.ptr);
        std::thread t(&EventAction::action, event_action);
        this->threads.push_back(std::move(t));
    }
}

void EventLoop::Stop()
{
    close(this->epoll_fd);
    for (auto &t : this->threads)
    {
        if(t.joinable())
            t.detach();
    }
}
