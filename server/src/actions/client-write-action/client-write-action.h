#pragma once

#include <queue>
#include "../event-action.h"
#include <csignal>
#include <errno.h>
#include <string.h>
#include <mutex>
#include "../../structs/request.h"
#include "../../structs/response.h"
#include "../../request-util/serialization/serialization.h"

class ClientWriteAction : public EventAction
{
public:
   ClientWriteAction(int fd, int efd);
   ~ClientWriteAction();
   void action();
   void addToEpollIfNotExists();
   void addMessage(request req);
   void addResponse(response res);
   int orginal_fd;
private:
   std::mutex data_mtx;
   std::deque<std::string> data_to_send = {};
   bool in_epoll = false;
};