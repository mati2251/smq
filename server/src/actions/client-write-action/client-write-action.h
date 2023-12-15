#pragma once

#include <queue>
#include "../event-action.h"
#include <csignal>
#include "../../structs/message.h"

class ClientWriteAction : public EventAction{
 public:
    ClientWriteAction(int fd, int efd);
    void action();
    void addToEpoll();
 private:
    std::queue<message> messages = {};

};