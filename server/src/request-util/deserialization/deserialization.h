#pragma once

#include <string>
#include "../../structs/request.h"

request deserializeRequest(const std::string &req_str);
