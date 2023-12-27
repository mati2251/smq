#include "message-handler.h"

void MessageHandler::handle(request req)
{
    message msg = deserializeMessage(req.body);
    Topic *topic = ServerState::getInstance().getTopic(msg.topic);
    if (!topic->checkIfPublisher(req.from))
    {
        throw ClientNotPublisherException(req.from, msg.topic);
    }
    topic->publish(msg);
    std::cout << "Message received from " << req.from << " to topic " << msg.topic << std::endl;
}

void MessageHandler::setEfd(int efd)
{
    this->efd = efd;
}
