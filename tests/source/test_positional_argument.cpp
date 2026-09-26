#include "argument_test_fixture.hpp"
#include "doctest.h"
#include "utility.hpp"

using namespace argon_testing;
using namespace argon::nargs;

using argon::argument_name;
using argon::parsing_failure;
using argon::positional_argument;
using argon::detail::parameter_descriptor;

TEST_SUITE_BEGIN("test_positional_argument");

namespace {

constexpr std::string_view help_msg = "test help msg";

const std::string name_value = "test";
const argument_name arg_name{name_value};

using sut_value_type = int;
using sut_type = positional_argument<sut_value_type>;

const std::string empty_str = "";
const std::string invalid_value_str = "invalid value";

constexpr sut_value_type default_value = 0;
constexpr sut_value_type valid_value = 1;

const std::vector<sut_value_type> choices{1, 2, 3};
constexpr sut_value_type invalid_choice = 4;

const range non_default_range = range{1ull, choices.size()};

} // namespace

TEST_CASE_FIXTURE(argument_test_fixture, "name() should return the proper argument_name instance") {
    const auto sut = sut_type(arg_name);
    CHECK_EQ(sut.name(), arg_name);
}

TEST_CASE_FIXTURE(argument_test_fixture, "help() should return an empty string by default") {
    const auto sut = sut_type(arg_name);
    CHECK(sut.help().empty());
}

TEST_CASE_FIXTURE(argument_test_fixture, "help() should return a massage set for the argument") {
    auto sut = sut_type(arg_name);
    sut.help(help_msg);
    CHECK_EQ(sut.help(), help_msg);
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "help_builder(verbose=false) should return an help_builder with no params"
) {
    constexpr bool verbose = false;

    auto sut = sut_type(arg_name);

    auto bld = get_help_builder(sut, verbose);
    REQUIRE_EQ(bld.name, sut.name().str());
    CHECK(bld.help.empty());
    CHECK(bld.params.empty());

    // with a help msg
    sut.help(help_msg);
    bld = get_help_builder(sut, verbose);
    REQUIRE_EQ(bld.name, sut.name().str());
    CHECK_EQ(bld.help, help_msg);
    CHECK(bld.params.empty());
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "help_builder(verbose=true) should return an help_builder with non-default params"
) {
    constexpr bool verbose = true;

    auto sut = sut_type(arg_name);

    auto bld = get_help_builder(sut, verbose);
    REQUIRE_EQ(bld.name, sut.name().str());
    CHECK(bld.help.empty());
    CHECK(bld.params.empty());

    // with a help msg
    sut.help(help_msg);

    bld = get_help_builder(sut, verbose);
    CHECK_EQ(bld.help, help_msg);
    CHECK(bld.params.empty());

    // other parameters
    sut.required(false);
    sut.suppress_arg_checks();
    sut.suppress_group_checks();
    sut.nargs(non_default_range);
    sut.choices(choices);
    sut.default_values(default_value);

    // check the descriptor parameters
    bld = get_help_builder(sut, verbose);

    const auto required_it = std::ranges::find(bld.params, "required", &parameter_descriptor::name);
    REQUIRE_NE(required_it, bld.params.end());
    CHECK_EQ(required_it->value, "false");

    const auto suppress_arg_checks_it =
        std::ranges::find(bld.params, "suppress arg checks", &parameter_descriptor::name);
    REQUIRE_NE(suppress_arg_checks_it, bld.params.end());
    CHECK_EQ(suppress_arg_checks_it->value, "true");

    const auto suppress_group_checks_it =
        std::ranges::find(bld.params, "suppress group checks", &parameter_descriptor::name);
    REQUIRE_NE(suppress_group_checks_it, bld.params.end());
    CHECK_EQ(suppress_group_checks_it->value, "true");

    const auto nargs_it = std::ranges::find(bld.params, "nargs", &parameter_descriptor::name);
    REQUIRE_NE(nargs_it, bld.params.end());
    CHECK_EQ(nargs_it->value, argon::util::as_string(non_default_range));

    const auto choices_it = std::ranges::find(bld.params, "choices", &parameter_descriptor::name);
    REQUIRE_NE(choices_it, bld.params.end());
    CHECK_EQ(choices_it->value, argon::util::join(choices, ", "));

    const auto default_value_it =
        std::ranges::find(bld.params, "default value(s)", &parameter_descriptor::name);
    REQUIRE_NE(default_value_it, bld.params.end());
    CHECK_EQ(default_value_it->value, std::to_string(default_value));
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "is_hidden() should return false by default or the value passed in the attribute setter"
) {
    auto sut = sut_type(arg_name);
    REQUIRE_FALSE(sut.is_hidden());

    sut.hidden();
    CHECK(sut.is_hidden());
}

TEST_CASE_FIXTURE(argument_test_fixture, "is_required() should return true by default") {
    auto sut = sut_type(arg_name);
    CHECK(sut.is_required());
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "is_required() should return the value set using the `required` param setter"
) {
    auto sut = sut_type(arg_name);

    sut.required(false);
    CHECK_FALSE(sut.is_required());

    sut.required();
    CHECK(sut.is_required());
}

TEST_CASE_FIXTURE(
    argument_test_fixture, "required(true) should throw if an argument is supressing"
) {
    auto sut = sut_type(arg_name);
    sut.required(false);

    SUBCASE("suppressing argument checks") {
        sut.suppress_arg_checks();
    }
    SUBCASE("suppressing argument group checks") {
        sut.suppress_group_checks();
    }
    SUBCASE("suppressing all checks") {
        sut.suppress_arg_checks();
        sut.suppress_group_checks();
    }

    CAPTURE(sut);

    CHECK_THROWS_WITH_AS(
        sut.required(true),
        std::format("A suppressing argument [{}] cannot be required!", arg_name.str()).c_str(),
        argon::invalid_configuration
    );
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "suppresses_arg_checks() should return the value set using the `suppress_arg_checks` param "
    "setter if the argument is not required"
) {
    auto sut = sut_type(arg_name);

    CHECK_THROWS_WITH_AS(
        sut.suppress_arg_checks(true),
        std::format("A required argument [{}] cannot suppress argument checks!", arg_name.str())
            .c_str(),
        argon::invalid_configuration
    );

    sut.required(false);

    sut.suppress_arg_checks(true);
    CHECK(sut.suppresses_arg_checks());

    sut.suppress_arg_checks(false);
    CHECK_FALSE(sut.suppresses_arg_checks());
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "suppresses_group_checks() should return the value set using the `suppress_group_checks` param "
    "setter if the argument is not required"
) {
    auto sut = sut_type(arg_name);

    CHECK_THROWS_WITH_AS(
        sut.suppress_group_checks(true),
        std::format(
            "A required argument [{}] cannot suppress argument group checks!", arg_name.str()
        )
            .c_str(),
        argon::invalid_configuration
    );

    sut.required(false);

    sut.suppress_group_checks(true);
    CHECK(sut.suppresses_group_checks());

    sut.suppress_group_checks(false);
    CHECK_FALSE(sut.suppresses_group_checks());
}

TEST_CASE_FIXTURE(argument_test_fixture, "is_used() should return false by default") {
    const auto sut = sut_type(arg_name);
    CHECK_FALSE(sut.is_used());
}

TEST_CASE_FIXTURE(
    argument_test_fixture, "is_used() should return true when argument contains a value"
) {
    auto sut = sut_type(arg_name);
    REQUIRE_FALSE(sut.is_used());

    set_value(sut, valid_value);
    CHECK(sut.is_used());
}

TEST_CASE_FIXTURE(argument_test_fixture, "count() should return 0 by default") {
    const auto sut = sut_type(arg_name);
    CHECK_EQ(sut.count(), 0ull);
}

TEST_CASE_FIXTURE(argument_test_fixture, "count() should return 1 when argument contains a value") {
    auto sut = sut_type(arg_name);
    set_value(sut, valid_value);

    CHECK_EQ(sut.count(), 1ull);
}

TEST_CASE_FIXTURE(argument_test_fixture, "has_value() should return false by default") {
    const auto sut = sut_type(arg_name);
    CHECK_FALSE(sut.has_value());
}

TEST_CASE_FIXTURE(argument_test_fixture, "has_value() should return true if the value is set") {
    auto sut = sut_type(arg_name);
    set_value(sut, valid_value);

    CHECK(sut.has_value());
}

TEST_CASE_FIXTURE(
    argument_test_fixture, "has_value() should return true if the default value is set"
) {
    auto sut = sut_type(arg_name);
    sut.default_values(default_value);

    CHECK(sut.has_value());
}

TEST_CASE_FIXTURE(argument_test_fixture, "has_parsed_values() should return false by default") {
    const auto sut = sut_type(arg_name);
    CHECK_FALSE(sut.has_parsed_values());
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "has_parsed_values() should return false regardless of the default value parameter"
) {
    auto sut = sut_type(arg_name);
    sut.default_values(default_value);

    CHECK_FALSE(sut.has_parsed_values());
}

TEST_CASE_FIXTURE(argument_test_fixture, "has_parsed_values() should true if the value is set") {
    auto sut = sut_type(arg_name);
    set_value(sut, valid_value);

    CHECK(sut.has_parsed_values());
}

TEST_CASE_FIXTURE(argument_test_fixture, "has_predefined_values() should return false by default") {
    const auto sut = sut_type(arg_name);
    CHECK_FALSE(sut.has_predefined_values());
}

TEST_CASE_FIXTURE(
    argument_test_fixture, "has_predefined_values() should return true if the default value is set"
) {
    auto sut = sut_type(arg_name);
    sut.default_values(default_value);

    CHECK(sut.has_predefined_values());
}

TEST_CASE_FIXTURE(
    argument_test_fixture, "value() should throw if the argument's value has not been set"
) {
    auto sut = sut_type(arg_name);

    REQUIRE_FALSE(sut.has_value());
    CHECK_THROWS_AS(discard(sut.value()), std::logic_error);
}

TEST_CASE_FIXTURE(
    argument_test_fixture, "value() should return the argument's value if it has been set"
) {
    auto sut = sut_type(arg_name);
    set_value(sut, valid_value);

    REQUIRE(sut.has_value());
    CHECK_EQ(sut.value(), valid_value);
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "value() should return the default argument's default value if it has been set and no values "
    "were parsed"
) {
    auto sut = sut_type(arg_name);
    sut.default_values(default_value);

    REQUIRE(sut.has_value());
    CHECK_EQ(sut.value(), default_value);
}

TEST_CASE_FIXTURE(
    argument_test_fixture, "value() should return the argument's parsed value if it has been set"
) {
    auto sut = sut_type(arg_name);
    sut.default_values(default_value);
    set_value(sut, valid_value);

    REQUIRE(sut.has_value());
    CHECK_EQ(sut.value(), valid_value);
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "value_or() should return the fallback value if the argument's value has not been set"
) {
    auto sut = sut_type(arg_name);
    constexpr sut_value_type fallback_value = 999;

    REQUIRE_FALSE(sut.has_value());
    CHECK_EQ(sut.value_or(fallback_value), fallback_value);
}

TEST_CASE_FIXTURE(
    argument_test_fixture, "value_or() should return the argument's value if it has been set"
) {
    auto sut = sut_type(arg_name);
    set_value(sut, valid_value);
    constexpr sut_value_type fallback_value = 999;

    REQUIRE(sut.has_value());
    CHECK_EQ(sut.value_or(fallback_value), valid_value);
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "value_or() should return the default argument's default value if it has been set and no "
    "values "
    "were parsed"
) {
    auto sut = sut_type(arg_name);
    sut.default_values(default_value);
    constexpr sut_value_type fallback_value = 999;

    REQUIRE(sut.has_value());
    CHECK_EQ(sut.value_or(fallback_value), default_value);
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "set_value(any) should throw when the given string cannot be converted to an instance of "
    "value_type"
) {
    auto sut = sut_type(arg_name);

    SUBCASE("given string is empty") {
        REQUIRE_THROWS_WITH_AS(
            set_value(sut, empty_str),
            invalid_value_msg(arg_name, empty_str).c_str(),
            parsing_failure
        );
        CHECK_FALSE(sut.has_value());
    }

    SUBCASE("given string is non-convertible to value_type") {
        REQUIRE_THROWS_WITH_AS(
            set_value(sut, invalid_value_str),
            invalid_value_msg(arg_name, invalid_value_str).c_str(),
            parsing_failure
        );
        CHECK_FALSE(sut.has_value());
    }
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "set_value(any) should throw when the choices set does not contain the parsed value"
) {
    auto sut = sut_type(arg_name);
    sut.choices(choices);

    REQUIRE_THROWS_WITH_AS(
        set_value(sut, invalid_choice),
        doctest::Contains(invalid_choice_msg(arg_name, as_string(invalid_choice)).c_str()),
        parsing_failure
    );
    CHECK_FALSE(sut.has_value());
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "set_value(any) should throw when adding the given value would result in exceeding the maximum "
    "number of values specified by nargs"
) {
    auto sut = sut_type(arg_name).nargs(non_default_range);

    for (const auto value : choices)
        REQUIRE_NOTHROW(set_value(sut, value));

    REQUIRE_EQ(sut.values(), choices);

    CHECK_THROWS_WITH_AS(
        set_value(sut, valid_value),
        parsing_failure::invalid_nvalues(arg_name, std::weak_ordering::greater).what(),
        parsing_failure
    );
}

TEST_CASE_FIXTURE(argument_test_fixture, "set_value(any) should perform the specified action") {
    auto sut = sut_type(arg_name);

    SUBCASE("observe action") {
        const auto is_power_of_two = [](const sut_value_type n) {
            if (not ((n > 0) and (n & (n - 1)) == 0)) {
                throw std::runtime_error(std::format("Value `{}` is not a power of 2", n));
            }
        };

        sut.action<argon::action_type::observe>(is_power_of_two);

        CHECK_THROWS_AS(set_value(sut, 3), std::runtime_error);

        sut_value_type valid_value = 16;
        REQUIRE_NOTHROW(set_value(sut, valid_value));
        CHECK_EQ(sut.value(), valid_value);
    }

    SUBCASE("transform action") {
        const auto double_action = [](const sut_value_type& value) { return 2 * value; };
        sut.action<argon::action_type::transform>(double_action);

        set_value(sut, valid_value);

        CHECK_EQ(sut.value(), double_action(valid_value));
    }

    SUBCASE("modify action") {
        const auto double_action = [](sut_value_type& value) { value *= 2; };
        sut.action<argon::action_type::modify>(double_action);

        auto test_value = valid_value;

        set_value(sut, test_value);

        double_action(test_value);
        CHECK_EQ(sut.value(), test_value);
    }
}

TEST_CASE_FIXTURE(
    argument_test_fixture, "nvalues_ordering() should return less for default nargs (1)"
) {
    const auto sut = sut_type(arg_name);
    CHECK(std::is_lt(nvalues_ordering(sut)));
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "nvalues_ordering() should return equivalent if a default value has been set"
) {
    auto sut = sut_type(arg_name);
    sut.nargs(non_default_range);

    sut.default_values(default_value);

    CHECK(std::is_eq(nvalues_ordering(sut)));
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "nvalues_ordering() should return equivalent only when the number of values "
    "is in the specified range"
) {
    auto sut = sut_type(arg_name);
    sut.nargs(non_default_range);

    REQUIRE(std::is_lt(nvalues_ordering(sut)));

    for (const auto value : choices) {
        REQUIRE_NOTHROW(set_value(sut, value));
        CHECK(std::is_eq(nvalues_ordering(sut)));
    }

    set_value_force(sut, invalid_choice);
    CHECK(std::is_gt(nvalues_ordering(sut)));
}

TEST_SUITE_END(); // test_positional_argument
