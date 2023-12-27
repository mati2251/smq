#pragma once
#include "../structs/request.h"

class Handler
{
public:
  virtual void handle(request req) = 0;
  virtual void setEfd(int efd) = 0;
protected:
  int efd;
};