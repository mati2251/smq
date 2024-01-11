#include "client-write-action.h"
#include <climits>

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
    if (this->requests.empty() && this->responses.empty())
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
    std::string data = "";
    if (this->responses.empty())
        this->sendExchange(&this->requests);
    else
        this->sendExchange(&this->responses);
    int err = epoll_ctl(this->efd, EPOLL_CTL_MOD, this->fd, &this->ev);
    if (err == -1)
    {
        std::cerr << "epoll_ctl: ClientWriteAction" << std::endl;
        std::cerr << strerror(errno) << std::endl;
    }
}

void ClientWriteAction::sendExchange(std::queue<package> *data)
{
    package exchange = data->front();
    if( (((double)(clock() - exchange.time))/CLOCKS_PER_SEC) < get_package_lifetime_conf() || get_package_lifetime_conf() == 0){
        int size = write(fd, const_cast<char *>(exchange.s.c_str()), exchange.s.size());
        if (size == -1 )
        {
            std::cerr << "Error message write" << std::endl;
            return;
        }
        if ((size_t)size == exchange.s.size())
        {
            data->pop();
        }
        else
        {
            data->front().s = exchange.s.substr(size);
            data->front().time = clock();
        }
    } else {
        std::cout << "Package lifetime was violated. Package ignored" << std::endl;
        data->pop();
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
    if (get_buffer_size_conf() >= requests.size() || get_buffer_size_conf() == 0){
        package pack;
        pack.s = serialize(req);
        pack.time = clock();
        std::lock_guard<std::mutex> lock(this->data_mtx);
        this->requests.push(pack);
        this->addToEpollIfNotExists();
    } else {
        throw FullBufferException();
    }
}

void ClientWriteAction::addResponse(response res)
{
    package pack;
    pack.s = serialize(res);
    pack.time = clock();
    std::lock_guard<std::mutex> lock(this->data_mtx);
    this->responses.push(pack);
    this->addToEpollIfNotExists();
}
