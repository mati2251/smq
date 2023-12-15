#include "message-handler.h"

void MessageHandler::handle(request req)
{
    message msg = deserializeMessage(req.body);
    std::cout << "MessageHandler: " << msg.topic << " " << msg.content << std::endl;
    ClientWriteAction *action = new ClientWriteAction(req.from, this->efd);
    action->addToEpoll();
}

void MessageHandler::setEfd(int efd)
{
    this->efd = efd;
}
