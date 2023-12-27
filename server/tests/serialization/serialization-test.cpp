
#include <catch2/catch_test_macros.hpp>
#include "../../../src/structs/request.h"
#include "../../../src/request-util/serialization/serialization.h"
TEST_CASE("request serialization", "[serialization]")
{
    request req = {request_type::MESSAGE, 1, "topic", "content"};
    REQUIRE(serialize(req )== "MESSAGE\n1\ntopic\ncontent\n\n");
}

TEST_CASE("response serialization", "[serialization]")
{
    response res;
    res.code = response_code::SUCCESS;
    res.request_id = 1;
    res.message = "message";
    REQUIRE(serialize(res) == "RESPONSE\n1\n0\nmessage\n\n");
}