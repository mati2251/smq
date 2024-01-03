#include "message-handler.h"

void MessageHandler::handle(request req)
{
     
    Topic *topic = ServerState::getInstance().getTopic(req.topic);
    if (!topic->checkIfPublisher(req.from))
    {
        throw ClientNotPublisherException(req.from, req.topic);
    }
    topic->publish(req);
    std::cout << "Message received from " << req.from << " to topic " << req.topic << std::endl;
}

void MessageHandler::setEfd(int efd)
{
    this->efd = efd;
}
