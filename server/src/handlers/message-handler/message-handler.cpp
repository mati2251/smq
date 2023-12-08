#include "message-handler.h"

void MessageHandler::handle(request req)
{
    std::cout << req.body << std::endl;

}

void MessageHandler::setEfd(int efd)
{
    this->efd = efd;
}
