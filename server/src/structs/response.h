#pragma once
#include <string>

enum response_code{
    SUCCESS, // 0
    CLIENT_NOT_PUBLISHER, // 1
    CLIENT_NOT_SUBSCRIBER, // 2
    TOPIC_NOT_FOUND, // 3
    CLIENT_ALREADY_PUBLISHER, // 4
    CLIENT_ALREADY_SUBSCRIBER, // 5
    INVALID_REQUEST, // 6
    ACTION_UNKNOWN, // 7
    UNKNOWN_ERROR, // 8
    FULL_BUFFER // 9
};

struct response 
{
    response_code code;
    int request_id;
    std::string message;
};
