#pragma once

#include "../../structs/request.h"
#include "../handler.h"
#include <iostream>
#include "../../json/deserialization/deserialization.h"
#include "action-exception.hpp"
#include "../../topic/topic.h"
#include "../../server-state/server-state.h"

class ActionHandler : public Handler
{
public:
    void handle(request req);
    void setEfd(int efd);
private:
    void hanldeSubsribeAction(action_topic act);
    void handleUnsubscribeAction(action_topic act);
    void handlePublishAction(action_topic act);
    void handleUnpublishAction(action_topic act);
    ClientWriteAction *getClientWriteAction(int client_id);
};