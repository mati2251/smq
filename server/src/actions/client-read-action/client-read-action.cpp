#include "client-read-action.h"

ClientReadAction::ClientReadAction(int fd, int efd) : EventAction(fd, efd)
{
    this->ev.data.ptr = this;
    this->ev.events = EPOLLIN | EPOLLONESHOT;
}

ClientReadAction::~ClientReadAction()
{
}

void ClientReadAction::action()
{
    try
    {
        readRequest();
        if (checkEndOfRequest())
        {
            int endOfRequest = buffer.find("\n\n") + 2;
            std::string request = buffer.substr(0, endOfRequest);
            buffer = buffer.substr(endOfRequest + 1);
            RequestHandler::getInstance().handle(request, this->fd);
        }
    }
    catch (ConnectionCloseException &e)
    {
        closeConnection();
    }
    int err = epoll_ctl(this->efd, EPOLL_CTL_MOD, this->fd, &this->ev);
    if (err == -1)
    {
        throw std::runtime_error("Epoll ctl mod error");
    }
}

void ClientReadAction::readRequest()
{
    char buf[BUFFER_SIZE];
    int n = read(this->fd, buf, BUFFER_SIZE);
    if (n == 0)
    {
        throw ConnectionCloseException();
    }
    else if (n < 0)
    {
        return;
    }
    buffer += std::string(buf, n);
}

bool ClientReadAction::checkEndOfRequest()
{
    return buffer.find("\n\n") != std::string::npos;
}

void ClientReadAction::closeConnection()
{
    close(this->fd);
    ServerState::getInstance().removeClient(this->fd);
    delete this;
}
