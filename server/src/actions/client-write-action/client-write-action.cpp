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
    if (!this->responses.empty())
    {
        send(this->responses, this->responses_mtx);
        std::cout << "Sending response to client " << this->orginal_fd << std::endl;
        return;
    }
    if (!this->messages.empty())
    {
        send(this->messages, this->messages_mtx);
        std::cout << "Sending message to client " << this->orginal_fd << std::endl;
    }
}

template <typename T>
void ClientWriteAction::send(std::queue<T> &q, std::mutex &mtx)
{
    std::string msg = serialize(q.front());
    int size = write(fd, const_cast<char *>(msg.c_str()), msg.size());
    if (size == -1)
    {
        std::cerr << "write: ClientWriteAction" << std::endl;
        std::cerr << strerror(errno) << std::endl;
        return;
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        q.pop();
        if (this->messages.empty() && this->responses.empty())
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
            std::cerr << strerror(errno) << std::endl;
        }
    }
}

void ClientWriteAction::addToEpollIfNotExists()
{
    std::lock_guard<std::mutex> lock(this->messages_mtx);
    std::lock_guard<std::mutex> lock2(this->responses_mtx);
    if (this->in_epoll)
        return;
    this->in_epoll = true;
    int t = epoll_ctl(this->efd, EPOLL_CTL_ADD, this->fd, &this->ev);
    if (t == -1)
    {
        std::cerr << "epoll_ctl: ClientWriteAction" << std::endl;
        std::cerr << strerror(errno) << std::endl;
    }
}

void ClientWriteAction::addMessage(message msg)
{
    std::lock_guard<std::mutex> lock(this->messages_mtx);
    this->messages.push(msg);
}

void ClientWriteAction::addResponse(response res)
{
    std::lock_guard<std::mutex> lock(this->responses_mtx);
    this->responses.push(res);
}