#pragma once
#include <sys/epoll.h>
#include <iostream>
#include "../actions/accept-action/accept-action.h"
#include <thread>
#include <vector>
#include <list>

class EventLoop
{
public:
    EventLoop(int &sock);
    ~EventLoop();
    std::list<std::thread> threads = {};

    void Run();
    void Stop();

private:
    int sock;
    int epoll_fd;
};