#include "deserialization.h"

std::pair<std::string, std::string> splitToTwice(const std::string &str, std::string delim)
{
    std::pair<std::string, std::string> result;
    size_t pos = str.find(delim);
    if (pos == std::string::npos)
    {
        throw DeserializationException("Split error");
    }
    result.first = str.substr(0, pos);
    result.second = str.substr(pos + delim.length());
    return result;
}

request deserializeRequest(const std::string &req_str)
{
    request result;
    auto split_result = splitToTwice(req_str, "\n");
    result.type = getRequestType(split_result.first);
    split_result = splitToTwice(split_result.second, "\n");
    result.id = atoi(split_result.first.c_str());
    split_result = splitToTwice(split_result.second, "\n");
    result.topic = split_result.first;
    split_result = splitToTwice(split_result.second, "\n\n");
    result.body = split_result.first;
    result.from = 0;
    return result;
}
