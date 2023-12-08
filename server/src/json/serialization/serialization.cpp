#include "serialization.h"

std::string serializeMessage(const message &msg)
{
    std::string result = "{";
    result += "\"topic\": \"" + msg.topic + "\",";
    result += "\"content\": \"" + msg.content + "\"";
    result += "}";
    return result;
}
