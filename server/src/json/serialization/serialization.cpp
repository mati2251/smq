#include "serialization.h"

std::string serializeMessage(const message &msg)
{
    std::string result = "MESSAGE\n{";
    result += "\"topic\": \"" + msg.topic + "\",";
    result += "\"content\": \"" + msg.content + "\"";
    result += "}";
    return result;
}

std::string serializeResponse(const response &resp)
{
    std::string result = "RESPONSE\n{";
    result += "\"code\": " + std::to_string(resp.code) + ",";
    result += "\"message\": \"" + resp.message + "\"";
    result += "}";
    return result;
}