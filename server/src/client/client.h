#pragma once

#include <set>
#include <string>

class Client
{
public:
    Client();
    ~Client();
    void read();
    void write();
private:
    std::set<std::string> topics = {};
    void topicActionHandle();
    void messageHandle();
};
