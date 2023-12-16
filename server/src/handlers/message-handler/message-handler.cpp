#include "message-handler.h"

void MessageHandler::handle(request req)
{
    message msg = deserializeMessage(req.body);
    Topic *topic = ServerState::getInstance().getTopic(msg.topic);
    if (topic == nullptr || !topic->checkIfPublisher(req.from))
    {
        std::cerr << "Client " << req.from << " tried to send to not exist topic or client isn't register as publisher" << msg.topic << std::endl;
        return;
    }
    topic->publish(msg);
    std::cout << "Message received from " << req.from << " to topic " << msg.topic << std::endl;
}

void MessageHandler::setEfd(int efd)
{
    this->efd = efd;
}
