#include "serialization.h"
#include "../request-util.h"

std::string serializeRequest(const request &req) {
    std::string result = toString(req.type) + "\n";
    result += std::to_string(req.id) + "\n";
    result += req.topic + "\n";
    result += req.body + "\n\n";
    return result;
}

std::string serializeResponse(const response &resp) {
    std::string result = "RESPONSE\n";
    result += std::to_string(resp.request_id) + "\n";
    result += std::to_string(resp.code) + "\n";
    result += resp.message + "\n\n";
    return result;
}

template<typename T>
std::string serialize(const T &obj) {
    if constexpr (std::is_same_v<T, request>) {
        return serializeRequest(obj);
    } else if constexpr (std::is_same_v<T, response>) {
        return serializeResponse(obj);
    }
    return "";
}

template std::string serialize<request>(const request &obj);

template std::string serialize<response>(const response &obj);
