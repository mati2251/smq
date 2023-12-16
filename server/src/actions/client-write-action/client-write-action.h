#pragma once

#include <queue>
#include "../event-action.h"
#include <csignal>
#include "../../structs/message.h"
#include "../../json/serialization/serialization.h"

class ClientWriteAction : public EventAction
{
public:
   ClientWriteAction(int fd, int efd);
   ~ClientWriteAction();
   void action();
   void addToEpollIfNotExists();
   void addMessage(message msg);
   int orginal_fd;
private:
   std::queue<message> messages = {};
   bool in_epoll = false;
};