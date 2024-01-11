#pragma once

#include <string>
#include "../../structs/response.h"
#include "../../structs/request.h"

std::string serializiRequest(const request &req);

std::string serializeResponse(const response &resp);

template <typename T>
std::string serialize(const T &obj);
