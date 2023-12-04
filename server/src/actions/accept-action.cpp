#include "accept-action.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

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
};

epoll_event& AcceptAction::getEpollEvent()
{
    return this->ev;
}


AcceptAction::AcceptAction(int fd, int &epoll_fd) : EventAction(fd), epoll_fd(epoll_fd)
{
    this->ev.data.ptr = this;
    this->ev.events = EPOLLIN;
}