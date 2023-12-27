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

template <typename T>
std::string serialize(const T &obj)
{
    if constexpr (std::is_same<T, message>::value)
    {
        return serializeMessage(obj);
    }
    else if constexpr (std::is_same<T, response>::value)
    {
        return serializeResponse(obj);
    }
    return "";
}

template std::string serialize<message>(const message &obj);
template std::string serialize<response>(const response &obj);