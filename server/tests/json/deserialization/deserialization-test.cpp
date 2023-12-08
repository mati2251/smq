#include <catch2/catch_test_macros.hpp>
#include "../../../src/json/deserialization/deserialization.h"
#include "../../../src/structs/message.h"

TEST_CASE("message deserialization", "[deserialization]") {
    message msg = deserializeMessage("{\"topic\"\n \f :\"topic\",\"content\":\"content \n\t\"}");
    REQUIRE(msg.topic == "topic");
    REQUIRE(msg.content == "content \n\t");
}

TEST_CASE("message deserialization with empty content", "[deserialization]") {
    message msg = deserializeMessage("{\"topic\":\"topic\",\"content\":\"\"}");
    REQUIRE(msg.topic == "topic");
    REQUIRE(msg.content == "");
}

TEST_CASE("message with bad json", "[deserialization]") {
    REQUIRE_THROWS_AS(deserializeMessage("{\"topic\",:\"topic\",\"content\":\"content\""), DeserializationException);
}

TEST_CASE("message with bad key", "[deserialization]") {
    REQUIRE_THROWS_AS(deserializeMessage("{\"topic2\":\"topic\",\"content\":\"content\""), DeserializationException);
}

TEST_CASE("message with too much parameters", "[deserialization]") {
    REQUIRE_THROWS_AS(deserializeMessage("{\"topic\":\"topic\",\"content\":\"content\", \"content\": \"topic\"}"), DeserializationException);
}

TEST_CASE("action topic deserialization", "[deserialization]") {
    action_topic action = deserializeAction("{\"topic\":\"topic\",\"action\":\"action\"}");
    REQUIRE(action.topic == "topic");
    REQUIRE(action.action == "action");
}

TEST_CASE("action topic deserialization with empty action", "[deserialization]") {
    action_topic action = deserializeAction("{\"topic\":\"topic\",\"action\":\"\"}");
    REQUIRE(action.topic == "topic");
    REQUIRE(action.action == "");
}

TEST_CASE("action topic with bad json", "[deserialization]") {
    REQUIRE_THROWS_AS(deserializeAction("{\"topic\",\"action\":\"action\"}"), DeserializationException);
}

TEST_CASE("action topic with bad key", "[deserialization]") {
    REQUIRE_THROWS_AS(deserializeAction("{\"topic2\":\"topic\",\"action\":\"action\"}"), DeserializationException);
}

TEST_CASE("action topic with too much parameters", "[deserialization]") {
    REQUIRE_THROWS_AS(deserializeAction("{\"topic\":\"topic\",\"action\":\"action\", \"action\": \"topic\"}"), DeserializationException);
}