#pragma once

#include "../../structs/request.h"
#include "../handler.h"
#include <iostream>
#include "action-exception.hpp"
#include "../../topic/topic.h"
#include "../../server-state/server-state.h"

class ActionHandler : public Handler
{
public:
    void handle(request req);
    void setEfd(int efd);
private:
    void hanldeSubsribeAction(request act);
    void handleUnsubscribeAction(request act);
    void handlePublishAction(request act);
    void handleUnpublishAction(request act);
    ClientWriteAction *getClientWriteAction(int client_id);
};