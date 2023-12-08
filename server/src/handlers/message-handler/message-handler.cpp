#include "message-handler.h"

void MessageHandler::handle(request req)
{
    message msg = deserializeMessage(req.body);
    std::cout << "MessageHandler: " << msg.topic << " " << msg.content << std::endl;
}

void MessageHandler::setEfd(int efd)
{
    this->efd = efd;
}
