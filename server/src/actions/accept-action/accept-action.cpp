#include "accept-action.h"

void AcceptAction::action()
{
    sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_fd = accept(this->fd, (sockaddr *)&client_addr, &client_addr_size);
    if (client_fd == -1)
    {
        std::cerr << "Accept failed" << std::endl;
    }
    std::cout << "Accepted connection from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;
    ClientReadAction* client_read_action = new ClientReadAction(client_fd, this->efd);
    epoll_event ev = client_read_action->getEpollEvent();
    if (epoll_ctl(this->efd, EPOLL_CTL_ADD, client_fd, &ev) == -1)
    {
        std::cerr << "Error epoll_ctl_add: " << std::strerror(errno) << std::endl;
    }
    epoll_ctl(this->efd, EPOLL_CTL_MOD, this->fd, &this->ev);
};

epoll_event &AcceptAction::getEpollEvent()
{
    return this->ev;
}

AcceptAction::AcceptAction(int fd, int efd) : EventAction(fd, efd)
{
    this->ev.data.ptr = this;
    this->ev.events = EPOLLIN | EPOLLONESHOT;
}