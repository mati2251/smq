#pragma once

#include <string>
#include "../structs/request.h"

const std::string request_stype[] = {"ACTION", "MESSAGE"};

request_type getRequestType(std::string request_type_str);