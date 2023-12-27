#include <catch2/catch_test_macros.hpp>
#include "../../../src/structs/request.h"
#include "../../../src/request-util/deserialization/deserialization.h"

TEST_CASE("request deserialization", "[deserialization]") {
    std::string req_str = "MESSAGE\n1\ntopic\ncontent\n\n";
    request req = deserializeRequest(req_str);
    REQUIRE(req.type == request_type::MESSAGE);
    REQUIRE(req.id == 1);
    REQUIRE(req.topic == "topic");
    REQUIRE(req.body == "content");
}

TEST_CASE("deserialization with empty content", "[deserialization]") {
    std::string req_str = "MESSAGE\n1\ntopic\n\n\n";
    request req = deserializeRequest(req_str);
    REQUIRE(req.type == request_type::MESSAGE);
    REQUIRE(req.id == 1);
    REQUIRE(req.topic == "topic");
    REQUIRE(req.body == "");
}

TEST_CASE("request deserialization exception", "[deserialization]") {
    std::string req_str = "MESSAGE\n1\ntopic\ncontent";
    REQUIRE_THROWS_AS(deserializeRequest(req_str), DeserializationException);
}