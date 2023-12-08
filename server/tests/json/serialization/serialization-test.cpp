
#include <catch2/catch_test_macros.hpp>
#include "../../../src/json/serialization/serialization.h"
#include "../../../src/structs/message.h"

TEST_CASE("message serialization", "[serialization]") {
    message msg;
    msg.topic = "topic";
    msg.content = "content";
    REQUIRE(serializeMessage(msg) == "{\"topic\": \"topic\",\"content\": \"content\"}");
}