#include "client-read-action.h"

ClientReadAction::ClientReadAction(int fd, int efd) : EventAction(fd, efd)
{
    this->ev.data.ptr = this;
    this->ev.events = EPOLLIN | EPOLLONESHOT;
}

void ClientReadAction::action()
{
    char buffer[1024];
    int n = read(this->fd, buffer, 1024);
    write(1, buffer, n);
    epoll_ctl(this->efd, EPOLL_CTL_MOD, this->fd, &this->ev);
}

epoll_event &ClientReadAction::getEpollEvent()
{
    return this->ev;
}
