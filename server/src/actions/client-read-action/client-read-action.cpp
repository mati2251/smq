#include "client-read-action.h"

ClientReadAction::ClientReadAction(int fd, int efd) : EventAction(fd, efd)
{
    this->ev.data.ptr = this;
    this->ev.events = EPOLLIN | EPOLLONESHOT;
}

void ClientReadAction::action()
{
    request req = readRequest();
    epoll_ctl(this->efd, EPOLL_CTL_MOD, this->fd, &this->ev);
    RequestHandler::getInstance().handle(req);
}


request ClientReadAction::readRequest()
{
    char buffer[BUFFER_SIZE];
    std::string buffer_str = "";
    request req = getType();
    if(req.body.empty()){
        int size = read(this->fd, buffer, BUFFER_SIZE);
        req.body = std::string(buffer, size);
    }
    buffer_str = req.body;
    int bracetCount = 0;
    int messageSize = req.body.size();
    while (true)
    {
        if (checkEndOfRequest(buffer_str, bracetCount))
        {
            break;
        }
        int size = read(this->fd, buffer, BUFFER_SIZE);
        messageSize += size;
        if (messageSize > MESSAGE_SIZE_KB * 1024)
        {
            throw InvalidRequestException();
        }
        buffer_str = std::string(buffer, size);
        req.body += buffer_str;
    }
    return req;
}

request ClientReadAction::getType(std::string buffer_str)
{
    request req = {};
    char buffer[TOPIC_SIZE];
    int size = read(this->fd, buffer, TOPIC_SIZE);
    buffer_str += std::string(buffer, size);
    std::size_t new_line_index = buffer_str.find('\n');
    if (new_line_index == std::string::npos){
        if (buffer_str.size() >= TOPIC_SIZE){
            throw InvalidRequestException();
        }
        return getType(buffer_str);
    }
    req.type = getRequestType(buffer_str.substr(0, new_line_index));
    if (req.type == -1)
    {
        throw InvalidRequestException();
    }
    req.body = buffer_str.substr(new_line_index + 1);
    req.from = this->fd;
    return req;
}

bool ClientReadAction::checkEndOfRequest(std::string partRequest, int &bracetCount)
{
    size_t left = partRequest.find('{');
    size_t right = partRequest.find('}');
    if (left == std::string::npos && right == std::string::npos)
    {
        return false;
    }
    else if (left > right)
    {
        bracetCount--;
        if (bracetCount == 0)
        {
            return true;
        }
        return checkEndOfRequest(partRequest.substr(right + 1), bracetCount);
    }
    else if (left < right)
    {
        bracetCount++;
        return checkEndOfRequest(partRequest.substr(left + 1), bracetCount);
    }
    throw InvalidRequestException();
}
