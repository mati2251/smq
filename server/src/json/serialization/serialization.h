#pragma once

#include <string>
#include "../../structs/message.h"
#include "../../structs/response.h"

std::string serializeMessage(const message &msg);

std::string serializeResponse(const response &resp);
