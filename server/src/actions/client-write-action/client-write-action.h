#pragma once

#include <queue>
#include "../event-action.h"
#include <mutex>
#include "../../structs/request.h"
#include "../../structs/response.h"
#include "../../structs/package.h"

class ClientWriteAction final : public EventAction
{
public:
   ClientWriteAction(int fd, int efd);
   ~ClientWriteAction() override;
   void action() override;
   void sendExchange(std::queue<package> *data) const;
   void addToEpollIfNotExists();
   void addMessage(const request &req);
   void addResponse(const response &res);
   int orginal_fd;
private:
   std::mutex data_mtx;
   std::queue<package> requests = {};
   std::queue<package> responses = {};
   bool in_epoll = false;
};
