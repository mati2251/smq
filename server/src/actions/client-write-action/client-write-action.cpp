#include "client-write-action.h"
#include <climits>
#include <csignal>
#include <cerrno>
#include <cstring>
#include "../../configure/configure.h"
#include "../../request-util/serialization/serialization.h"
#include <fcntl.h>
#include "full-buffer-exception.hpp"


ClientWriteAction::ClientWriteAction(const int fd, const int efd) : EventAction(dup(fd), efd) {
    this->ev.data.ptr = this;
    this->ev.events = EPOLLOUT | EPOLLONESHOT;
    this->orginal_fd = fd;
}

ClientWriteAction::~ClientWriteAction() {
    close(this->fd);
}

void ClientWriteAction::action() {
    std::lock_guard lock(this->data_mtx);
    if (this->requests.empty() && this->responses.empty()) {
        this->in_epoll = false;
        if (const int err = epoll_ctl(this->efd, EPOLL_CTL_DEL, this->fd, nullptr); err == -1) {
            std::cerr << "epoll_ctl: ClientWriteAction" << std::endl;
            std::cerr << strerror(errno) << std::endl;
        }
        return;
    }
    std::string data;
    if (this->responses.empty())
        this->sendExchange(&this->requests);
    else
        this->sendExchange(&this->responses);
    if (const int err = epoll_ctl(this->efd, EPOLL_CTL_MOD, this->fd, &this->ev); err == -1) {
        std::cerr << "epoll_ctl: ClientWriteAction" << std::endl;
        std::cerr << strerror(errno) << std::endl;
    }
}

void ClientWriteAction::sendExchange(std::queue<package> *data) const {
    auto [s, time] = data->front();
    if (static_cast<double>(std::time(nullptr) - time) < get_package_lifetime_conf() ||
        get_package_lifetime_conf() == 0) {
        if (const int size = write(fd, s.c_str(), s.size()); size == -1) {
            std::cerr<< "Error message write" << std::endl;
            std::cerr << strerror(errno) << std::endl;
        }
        else if (static_cast<size_t>(size) == s.size()) {
            data->pop();
        } else {
            data->front().s = s.substr(size);
            data->front().time = std::time(nullptr);
        }
    } else {
        std::cout << "Package lifetime was violated. Package ignored" << std::endl;
        data->pop();
    }
}

void ClientWriteAction::addToEpollIfNotExists() {
    if (this->in_epoll)
        return;
    this->in_epoll = true;
    if (const int t = epoll_ctl(this->efd, EPOLL_CTL_ADD, this->fd, &this->ev); t == -1) {
        std::cerr << "epoll_ctl: ClientWriteAction" << std::endl;
        std::cerr << strerror(errno) << std::endl;
    }
}

void ClientWriteAction::addMessage(const request &req) {
    if (get_buffer_size_conf() >= requests.size() || get_buffer_size_conf() == 0) {
        package pack;
        pack.s = serialize(req);
        pack.time = std::time(nullptr);
        std::lock_guard lock(this->data_mtx);
        this->requests.push(pack);
        this->addToEpollIfNotExists();
    } else {
        throw FullBufferException();
    }
}

void ClientWriteAction::addResponse(const response &res) {
    package pack;
    pack.s = serialize(res);
    pack.time = std::time(nullptr);
    std::lock_guard lock(this->data_mtx);
    this->responses.push(pack);
    this->addToEpollIfNotExists();
}
