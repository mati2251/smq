#pragma once

#include <thread>
#include <list>

class EventLoop
{
public:
    explicit EventLoop(const int &sock);
    ~EventLoop();
    std::list<std::thread> threads = {};

    void run();
    void stop();

private:
    int sock;
    int epoll_fd{};
};
