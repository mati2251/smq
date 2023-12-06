#pragma once
#include <sys/epoll.h>
#include <iostream>
#include "../actions/accept-action.h"
#include <thread>
#include <vector>

class EventLoop
{
public:
    EventLoop(int &sock);
    ~EventLoop();

    void Run();
    void Stop();

private:
    int sock;
    int epoll_fd;
};