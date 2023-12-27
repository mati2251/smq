
#include <catch2/catch_test_macros.hpp>
#include "../../../src/json/serialization/serialization.h"
#include "../../../src/structs/message.h"

TEST_CASE("message serialization", "[serialization]") {
    message msg;
    msg.topic = "topic";
    msg.content = "content";
    REQUIRE(serializeMessage(msg) == "MESSAGE\n{\"topic\": \"topic\",\"content\": \"content\"}");
}

TEST_CASE("response serialization", "[serialization]") {
    response res;
    res.code = response_code::SUCCESS;
    res.message = "message";
    REQUIRE(serializeResponse(res) == "RESPONSE\n{\"code\": 0,\"message\": \"message\"}");
}