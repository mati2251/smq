#pragma once

#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include "../event-action.h"
#include "../client-read-action/client-read-action.h"
#include "../client-write-action/client-write-action.h"
#include "../../server-state/server-state.h"
#include "accept-exception.hpp"

class AcceptAction : public EventAction
{
public:
    AcceptAction(int fd, int efd);
    void action();
private:
    void returnToEpoll();
};
