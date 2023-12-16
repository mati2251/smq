#include "client-write-action.h"

#include "client-write-action.h"

ClientWriteAction::ClientWriteAction(int fd, int efd) : EventAction(dup(fd), efd)
{
    this->ev.data.ptr = this;
    this->ev.events = EPOLLOUT | EPOLLONESHOT;
    this->orginal_fd = fd;
}

ClientWriteAction::~ClientWriteAction()
{
    close(this->fd);
}

void ClientWriteAction::action()
{
    const char *buf = serializeMessage(this->messages.front()).c_str();
    write(fd, const_cast<char *>(buf), sizeof(buf));
    this->messages.pop();
    if (this->messages.empty())
    {
        this->in_epoll = false;
        int err = epoll_ctl(this->efd, EPOLL_CTL_DEL, this->fd, &this->ev);
        if (err == -1)
        {
            std::cerr << "epoll_ctl: ClientWriteAction" << std::endl;
        }
        return;
    }
    int err = epoll_ctl(this->efd, EPOLL_CTL_MOD, this->fd, &this->ev);
    if (err == -1)
    {
        std::cerr << "epoll_ctl: ClientWriteAction" << std::endl;
    }
}

void ClientWriteAction::addToEpollIfNotExists()
{
    if (this->in_epoll)
        return;
    this->in_epoll = true;
    int t = epoll_ctl(this->efd, EPOLL_CTL_ADD, this->fd, &this->ev);
    if (t == -1)
    {
        std::cerr << "epoll_ctl: ClientWriteAction" << std::endl;
        perror("epoll_ctl");
    }
}

void ClientWriteAction::addMessage(message msg)
{
    this->messages.push(msg);
}