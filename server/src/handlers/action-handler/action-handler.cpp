#include "action-handler.h"

void ActionHandler::handle(request req)
{
    std::cout << req.body << std::endl;
}

void ActionHandler::setEfd(int efd)
{
    this->efd = efd;
}
