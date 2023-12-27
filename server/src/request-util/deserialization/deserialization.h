#pragma once 

#include <string>
#include "deserialization-exception.hpp"
#include "../../structs/request.h"
#include "../request-util.h"

request deserializeRequest(const std::string &req_str);