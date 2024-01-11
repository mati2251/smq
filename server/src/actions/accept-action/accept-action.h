#pragma once

#include "../event-action.h"


class AcceptAction final : public EventAction
{
public:
    AcceptAction(int fd, int efd);
    void action() override;
private:
    void returnToEpoll();
};
