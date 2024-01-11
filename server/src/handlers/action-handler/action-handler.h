#pragma once

#include "../../structs/request.h"
#include "../handler.h"

#include "../../server-state/server-state.h"

class ActionHandler : public Handler
{
public:
    void handle(request req) override;
    void setEfd(int efd) override;
private:
    void hanldeSubsribeAction(const request &act);
    void handleUnsubscribeAction(const request &act);
    void handlePublishAction(const request &act);
    void handleUnpublishAction(const request& act);
    ClientWriteAction *getClientWriteAction(int client_id);
};
