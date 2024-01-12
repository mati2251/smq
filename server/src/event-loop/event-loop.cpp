#include "event-loop.h"
#include "../handlers/request-handler/request-handler.h"
#include <sys/epoll.h>
#include <iostream>
#include <cerrno>
#include <cstring>
#include "../actions/accept-action/accept-action.h"

EventLoop::EventLoop(const int &sock) {
    this->sock = sock;
}

EventLoop::~EventLoop() {
    close(this->sock);
}

void EventLoop::run() {
    this->epoll_fd = epoll_create1(0);
    RequestHandler::getInstance().setEfd(this->epoll_fd);
    if (this->epoll_fd == -1) {
        std::cerr << "Error epoll_create: " << std::strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
    AcceptAction accept_action(this->sock, this->epoll_fd);
    epoll_event ev = accept_action.getEpollEvent();
    if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, this->sock, &ev) == -1) {
        std::cerr << "Error epoll_ctl_add: " << std::strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
    epoll_event events{};
    while (true) {
        epoll_wait(this->epoll_fd, &events, 1, -1);
        auto *event_action = static_cast<EventAction *>(events.data.ptr);
        std::thread t(&EventAction::action, event_action);
        this->threads.push_back(std::move(t));
    }
}

void EventLoop::stop() {
    for (auto &thread : this->threads) {
        thread.detach();
    }
    this -> threads.clear();
    close(this->epoll_fd);
    close(this->sock);
}
