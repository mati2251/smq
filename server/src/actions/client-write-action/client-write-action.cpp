#include "client-write-action.h"

#include "client-write-action.h"

ClientWriteAction::ClientWriteAction(int fd, int efd) : EventAction(dup(fd), efd)
{
    this->ev.data.ptr = this;
    this->ev.events = EPOLLOUT | EPOLLONESHOT;
}

void ClientWriteAction::action()
{
    char buf[10] = "Hello! wd";
    write(fd, buf, 10);
    epoll_ctl(this->efd, EPOLL_CTL_DEL, this->fd, &this->ev);
}

void ClientWriteAction::addToEpoll()
{
    int t = epoll_ctl(this->efd, EPOLL_CTL_ADD, this->fd, &this->ev);
    if (t == -1)
    {
        std::cerr << "epoll_ctl: ClientWriteAction" << std::endl;
        perror("epoll_ctl");
    }
}
