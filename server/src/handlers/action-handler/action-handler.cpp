#include "action-handler.h"

void ActionHandler::handle(request req)
{
    action_topic action = deserializeAction(req.body);
    std::cout << "ActionHandler: " << action.topic << " " << action.action << std::endl;
}

void ActionHandler::setEfd(int efd)
{
    this->efd = efd;
}
