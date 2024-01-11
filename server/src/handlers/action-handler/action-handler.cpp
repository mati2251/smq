#include "action-handler.h"
#include <iostream>
#include "action-exception.hpp"
#include "../../topic/topic.h"

void ActionHandler::handle(request req) {
    if (const std::string action_type = req.body.substr(0, req.body.find("\n\n")); action_type == "SUBSCRIBE") {
        this->hanldeSubsribeAction(req);
    } else if (action_type == "UNSUBSCRIBE") {
        this->handleUnsubscribeAction(req);
    } else if (action_type == "PUBLISH") {
        this->handlePublishAction(req);
    } else if (action_type == "UNPUBLISH") {
        this->handleUnpublishAction(req);
    } else {
        throw ActionUnknownException();
    }
}

void ActionHandler::setEfd(const int efd) {
    this->efd = efd;
}

void ActionHandler::hanldeSubsribeAction(const request &act) {
    Topic *t = ServerState::getInstance().addNewTopicIfNotExists(act.topic);
    const auto client = getClientWriteAction(act.from);
    if (client == nullptr) {
        std::cerr << "During handle subsribe action client not found" << std::endl;
        return;
    }
    t->addSubscriber(client);
}

void ActionHandler::handleUnsubscribeAction(const request &act) {
    Topic *t = ServerState::getInstance().getTopic(act.topic);
    t->removeSubscriber(act.from);
    if (t->isEmpty()) {
        ServerState::getInstance().removeTopic(act.topic);
    }
}

void ActionHandler::handlePublishAction(const request &act) {
    Topic *t = ServerState::getInstance().addNewTopicIfNotExists(act.topic);
    t->addPublisher(act.from);
}

void ActionHandler::handleUnpublishAction(const request &act) {
    Topic *t = ServerState::getInstance().getTopic(act.topic);
    t->removePublisher(act.from);
    if (t->isEmpty()) {
        ServerState::getInstance().removeTopic(act.topic);
    }
}

ClientWriteAction *ActionHandler::getClientWriteAction(const int client_id) {
    for (const auto client: ServerState::getInstance().clients) {
        if (client->orginal_fd == client_id) {
            return client;
        }
    }
    return nullptr;
}
