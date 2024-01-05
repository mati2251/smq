#include "request-util.h"
#include <unordered_map>

const std::unordered_map<std::string, request_type> request_type_map{
    {"ACTION", request_type::ACTION},
    {"MESSAGE", request_type::MESSAGE},
    {"INVALID", request_type::INVALID}};

request_type getRequestType(std::string request_type_str)
{
    if (request_type_map.find(request_type_str) == request_type_map.end())
    {
        return request_type::INVALID;
    }
    return request_type_map.at(request_type_str);
}

std::string toString(request_type type)
{
    auto it = request_type_map.begin();
    std::advance(it, type);
    return it->first;
}