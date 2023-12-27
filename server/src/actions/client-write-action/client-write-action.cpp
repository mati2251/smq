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
    std::lock_guard<std::mutex> lock(this->data_mtx);
    if (this->data_to_send.empty())
    {
        this->in_epoll = false;
        int err = epoll_ctl(this->efd, EPOLL_CTL_DEL, this->fd, nullptr);
        if (err == -1)
        {
            std::cerr << "epoll_ctl: ClientWriteAction" << std::endl;
            std::cerr << strerror(errno) << std::endl;
        }
        return;
    }
    std::string data = this->data_to_send.front();
    size_t size = write(fd, const_cast<char *>(data.c_str()), data.size());
    if (size == (size_t)-1)
    {
        std::cerr << "write: ClientWriteAction" << std::endl;
        std::cerr << strerror(errno) << std::endl;
    }
    if (size == data.size())
    {
        this->data_to_send.pop_front();
    }
    else
    {
        this->data_to_send.front() = data.substr(size);
    }
    int err = epoll_ctl(this->efd, EPOLL_CTL_MOD, this->fd, &this->ev);
    if (err == -1)
    {
        std::cerr << "epoll_ctl: ClientWriteAction" << std::endl;
        std::cerr << strerror(errno) << std::endl;
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
        std::cerr << strerror(errno) << std::endl;
    }
}

void ClientWriteAction::addMessage(request req)
{
    std::string s = serialize(req);
    std::lock_guard<std::mutex> lock(this->data_mtx);
    this->data_to_send.push_back(s);
    this->addToEpollIfNotExists();
}

void ClientWriteAction::addResponse(response res)
{
    std::string s = serialize(res);
    std::lock_guard<std::mutex> lock(this->data_mtx);
    this->data_to_send.push_front(s);
    this->addToEpollIfNotExists();
}