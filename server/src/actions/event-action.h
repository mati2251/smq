#include <iostream>
#include <sys/epoll.h>

class EventAction {
public:
    int fd;
    EventAction(int fd) : fd(fd) {};
    virtual void action() = 0;
    virtual epoll_event& getEpollEvent() = 0;
protected:
    epoll_event ev = {};
};
