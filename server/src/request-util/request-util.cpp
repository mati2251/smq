#include "request-util.h"
#include <unordered_map>

const std::unordered_map<std::string, request_type> request_type_map{
    {"ACTION", ACTION},
    {"MESSAGE", MESSAGE},
    {"INVALID", INVALID}};

request_type getRequestType(const std::string request_type_str)
{
    if (!request_type_map.contains(request_type_str))
    {
        return INVALID;
    }
    return request_type_map.at(request_type_str);
}

std::string toString(const request_type type)
{
    auto it = request_type_map.begin();
    std::advance(it, type);
    return it->first;
}
