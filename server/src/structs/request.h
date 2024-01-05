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
    request_type type = request_type::INVALID;
    int id;
    std::string topic;
    std::string body;
    int from;
};