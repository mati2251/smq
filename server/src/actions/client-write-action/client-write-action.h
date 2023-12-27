#pragma once

#include <queue>
#include "../event-action.h"
#include <csignal>
#include <errno.h>
#include <string.h>
#include "../../structs/message.h"
#include "../../json/serialization/serialization.h"
#include <mutex>

class ClientWriteAction : public EventAction
{
public:
   ClientWriteAction(int fd, int efd);
   ~ClientWriteAction();
   void action();
   void addToEpollIfNotExists();
   void addMessage(message msg);
   void addResponse(response res);
   template <typename T> void send(std::queue<T> &q, std::mutex &mtx);
   int orginal_fd;
private:
   std::mutex messages_mtx;
   std::mutex responses_mtx;
   std::queue<message> messages = {};
   std::queue<response> responses = {};
   bool in_epoll = false;
};