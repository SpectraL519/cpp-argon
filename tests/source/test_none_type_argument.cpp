#include "argument_test_fixture.hpp"
#include "doctest.h"

using namespace argon_testing;

TEST_SUITE_BEGIN("test_none_type_argument");

using argon::argument_name;
using argon::optional_argument;
using argon::parsing_failure;

namespace {

const std::string primary_name = "test";
const std::string secondary_name = "t";

const argument_name arg_name{primary_name, secondary_name};

using sut_value_type = argon::none_type;
using sut_type = optional_argument<sut_value_type>;

const std::string some_value = "some-value";

} // namespace

TEST_CASE_FIXTURE(
    argument_test_fixture, "mark_used should always return false (further values not accepted)"
) {
    auto sut = sut_type(arg_name);
    CHECK_FALSE(mark_used(sut));
}

TEST_CASE_FIXTURE(
    argument_test_fixture, "set_value should always throw and no values should be stored"
) {
    auto sut = sut_type(arg_name);

    CHECK_THROWS_WITH_AS(
        set_value(sut, some_value),
        std::format(
            "Cannot set values for a none-type argument '{}' (value: '{}')",
            arg_name.str(),
            some_value
        )
            .c_str(),
        parsing_failure
    );
}

TEST_SUITE_END(); // test_none_type_argument
