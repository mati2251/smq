#include "message-handler.h"

#include "../../topic/topic.h"
#include <iostream>
#include "../../topic/topic-exceptions.hpp"

void MessageHandler::handle(const request req) {
    Topic *topic = ServerState::getInstance().getTopic(req.topic);
    if (!topic->checkIfPublisher(req.from)) {
        throw ClientNotPublisherException(req.from, req.topic);
    }
    topic->publish(req);
    std::cout << "Message received from " << req.from << " to topic " << req.topic << std::endl;
}

void MessageHandler::setEfd(const int efd) {
    this->efd = efd;
}
