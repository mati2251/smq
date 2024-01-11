#include "client-read-action.h"
#include "../../request-util/request-util.h"
#include "connection-close-exception.hpp"
#include "../../handlers/request-handler/request-handler.h"
#include <csignal>

ClientReadAction::ClientReadAction(const int fd, const int efd) : EventAction(fd, efd) {
    this->ev.data.ptr = this;
    this->ev.events = EPOLLIN | EPOLLONESHOT;
}

ClientReadAction::~ClientReadAction() = default;

void ClientReadAction::action() {
    try {
        readRequest();
        if (checkEndOfRequest()) {
            const int endOfRequest = buffer.find("\n\n") + 2;
            const std::string request = buffer.substr(0, endOfRequest);
            buffer = buffer.substr(endOfRequest);
            RequestHandler::getInstance().handle(request, this->fd);
        }
        if (const int err = epoll_ctl(this->efd, EPOLL_CTL_MOD, this->fd, &this->ev); err == -1) {
            throw std::runtime_error("Epoll ctl mod error");
        }
    } catch (std::exception &_) {
        std::cout << "Client " << this->fd << " disconnected" << std::endl;
        closeConnection();
    }
}

void ClientReadAction::readRequest() {
    char buf[BUFFER_SIZE];
    const int n = read(this->fd, buf, BUFFER_SIZE);
    if (n == 0) {
        throw ConnectionCloseException();
    } if (n < 0) {
        return;
    }
    buffer += std::string(buf, n);
}

bool ClientReadAction::checkEndOfRequest() const {
    return buffer.find("\n\n") != std::string::npos;
}

void ClientReadAction::closeConnection() const {
    close(this->fd);
    ServerState::getInstance().removeClient(this->fd);
}
