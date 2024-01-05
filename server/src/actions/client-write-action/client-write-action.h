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
#include <fcntl.h>

class ClientWriteAction : public EventAction
{
public:
   ClientWriteAction(int fd, int efd);
   ~ClientWriteAction();
   void action() override;
   void sendExchange(std::queue<std::string> *data);
   void addToEpollIfNotExists();
   void addMessage(request req);
   void addResponse(response res);
   int orginal_fd;
private:
   std::mutex data_mtx;
   std::queue<std::string> requests = {};
   std::queue<std::string> responses = {};
   bool in_epoll = false;
};