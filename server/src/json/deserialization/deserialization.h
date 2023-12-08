#pragma once 

#include <string>
#include "../../structs/message.h"
#include "../../structs/action-topic.h"
#include "deserialization-exception.hpp"

message deserializeMessage(const std::string& json);

action_topic deserializeAction(const std::string& json);