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
    request_type type;
    std::string body;
    int from;
};
