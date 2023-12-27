#include "client-read-action.h"

ClientReadAction::ClientReadAction(int fd, int efd) : EventAction(fd, efd)
{
    this->ev.data.ptr = this;
    this->ev.events = EPOLLIN | EPOLLONESHOT;
    int flags = fcntl(this->fd, F_GETFL, 0);
    fcntl(this->fd, F_SETFL, flags | O_NONBLOCK);
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
            std::string request = buffer.substr(0, buffer.find_first_of("\n\n") + 2);
            buffer = buffer.substr(buffer.find_first_of("\n\n") + 2);
            int err = epoll_ctl(this->efd, EPOLL_CTL_MOD, this->fd, &this->ev);
            if (err == -1)
            {
                throw std::runtime_error("Epoll ctl mod error");
            }
            RequestHandler::getInstance().handle(request, this->fd);
        }
    }
    catch (ConnectionCloseException &e)
    {
        closeConnection();
    }
    catch (std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        closeConnection();
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
