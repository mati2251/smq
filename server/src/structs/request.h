#pragma once

#include <string>

enum request_type
{
    ACTION,
    MESSAGE,
    INVALID
};


struct request
{
    request_type type = INVALID;
    int id;
    std::string topic;
    std::string body;
    int from;
};
