#include "accept-action.h"
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include "../client-read-action/client-read-action.h"
#include "../client-write-action/client-write-action.h"
#include "../../server-state/server-state.h"

void AcceptAction::action() {
    sockaddr_in client_addr{};
    socklen_t client_addr_size = sizeof(client_addr);
    const int client_fd = accept(this->fd, reinterpret_cast<sockaddr *>(&client_addr), &client_addr_size);
    if (client_fd == -1) {
        std::cerr << "Accept error: " << std::strerror(errno) << std::endl;
        this->returnToEpoll();
        return;
    }
    std::cout << "Accepted connection from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) <<
            " as client " << client_fd << std::endl;
    auto *client_read_action = new ClientReadAction(client_fd, this->efd);
    epoll_event ev = client_read_action->getEpollEvent();
    if (epoll_ctl(this->efd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
        std::cerr << "Error epoll_ctl_add: " << std::strerror(errno) << std::endl;
    }
    this->returnToEpoll();
    ServerState::getInstance().addClient(new ClientWriteAction(client_fd, this->efd));
}

void AcceptAction::returnToEpoll() {
    if (const int err = epoll_ctl(this->efd, EPOLL_CTL_MOD, this->fd, &this->ev); err == -1) {
        std::cerr << "Error epoll_ctl_mod: " << std::strerror(errno) << std::endl;
    }
};

AcceptAction::AcceptAction(const int fd, const int efd) : EventAction(fd, efd) {
    this->ev.data.ptr = this;
    this->ev.events = EPOLLIN | EPOLLONESHOT;
}
