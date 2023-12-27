#pragma once

#include <string>
#include "../structs/request.h"

request_type getRequestType(std::string request_type_str);

std::string toString(request_type type);