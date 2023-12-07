#pragma once
#include <list>
#include <string>
#include "../actions/client-write-action/client-write-action.h"

class Topic
{
private:
    std::string name;
    std::list<ClientWriteAction *> subscribers;
    std::list<int> publishers;
};