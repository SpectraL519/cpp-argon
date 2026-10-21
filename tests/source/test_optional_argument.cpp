#include "argument_test_fixture.hpp"
#include "doctest.h"
#include "utility.hpp"

#include <argon/util/string.hpp>

using namespace argon_testing;
using namespace argon::nargs;

using argon::argument_name;
using argon::optional_argument;
using argon::parsing_failure;
using argon::detail::parameter_descriptor;

TEST_SUITE_BEGIN("test_optional_argument");

namespace {

constexpr char flag_char = '-';

const std::string primary_name = "test";
const std::string secondary_name = "t";

const argument_name arg_name{primary_name, secondary_name, flag_char};
const argument_name arg_name_primary{primary_name, "", flag_char};

constexpr std::string_view help_msg = "test help msg";

using sut_value_type = int;
using invalid_value_type = double;
using sut_type = optional_argument<sut_value_type>;

const std::string empty_str = "";
const std::string invalid_value_str = "invalid_value";

constexpr sut_value_type arbitrary_value = 1;
constexpr sut_value_type default_value = 0;
constexpr sut_value_type implicit_value = -1;

const std::vector<sut_value_type> choices{1, 2, 3};
constexpr sut_value_type invalid_choice = 4;

const range non_default_range = range{1ull, choices.size()};

} // namespace

TEST_CASE_FIXTURE(argument_test_fixture, "name() should return the proper argument_name instance") {
    SUBCASE("initialized with the primary name only") {
        const auto sut = sut_type(arg_name_primary);
        const auto name = sut.name();

        CHECK(name.match(primary_name));
        CHECK_FALSE(name.match(secondary_name));
    }

    SUBCASE("initialized with the primary and secondary names") {
        const auto sut = sut_type(arg_name);
        const auto name = sut.name();

        CHECK(name.match(primary_name));
        CHECK(name.match(secondary_name));
    }
}

TEST_CASE_FIXTURE(argument_test_fixture, "help() should return an empty string by default") {
    const auto sut = sut_type(arg_name_primary);
    CHECK(sut.help().empty());
}

TEST_CASE_FIXTURE(argument_test_fixture, "help() should return a massage set for the argument") {
    auto sut = sut_type(arg_name_primary);
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

    // with the required flag enabled
    sut.required();

    bld = get_help_builder(sut, verbose);
    const auto required_it = std::ranges::find(bld.params, "required", &parameter_descriptor::name);
    REQUIRE_NE(required_it, bld.params.end());
    CHECK_EQ(required_it->value, "true");

    // other parameters
    sut.required(false); // required for argument check suppressing
    sut.suppress_arg_checks();
    sut.suppress_group_checks();
    sut.nargs(non_default_range);
    sut.choices(choices);
    sut.default_values(default_value);
    sut.implicit_values(implicit_value);

    // check the descriptor parameters
    bld = get_help_builder(sut, verbose);

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

    const auto implicit_value_it =
        std::ranges::find(bld.params, "implicit value(s)", &parameter_descriptor::name);
    REQUIRE_NE(implicit_value_it, bld.params.end());
    CHECK_EQ(implicit_value_it->value, std::to_string(implicit_value));
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "is_hidden() should return false by default or the value passed in the attribute setter"
) {
    auto sut = sut_type(arg_name_primary);
    REQUIRE_FALSE(sut.is_hidden());

    sut.hidden();
    CHECK(sut.is_hidden());
}

TEST_CASE_FIXTURE(argument_test_fixture, "is_required() should return false by default") {
    auto sut = sut_type(arg_name_primary);
    CHECK_FALSE(sut.is_required());
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "is_required() should return the value set using the `required` param setter"
) {
    auto sut = sut_type(arg_name_primary);

    sut.required();
    CHECK(sut.is_required());

    sut.required(false);
    CHECK_FALSE(sut.is_required());
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
    "suppress_arg_checks() should return the value set using the `suppress_arg_checks` param "
    "setter if the argument is not required"
) {
    auto sut = sut_type(arg_name);

    sut.required(true);
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

    sut.required(true);
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

TEST_CASE_FIXTURE(argument_test_fixture, "negative() should properly set up the negative prefix") {
    auto sut = sut_type(arg_name_primary);
    sut.negative("no-");

    CHECK(sut.name().has_negative());
    CHECK_EQ(sut.name().negative_prefix(), "no-");
}

TEST_CASE_FIXTURE(argument_test_fixture, "is_used() should return false by default") {
    const auto sut = sut_type(arg_name_primary);
    CHECK_FALSE(sut.is_used());
}

TEST_CASE_FIXTURE(
    argument_test_fixture, "is_used() should return true if the argument's flag has been used"
) {
    auto sut = sut_type(arg_name_primary);
    REQUIRE_FALSE(sut.is_used());

    mark_used(sut);
    CHECK(sut.is_used());
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "is_used() should return true if the argument's negative flag has been used"
) {
    auto sut = sut_type(arg_name_primary);
    sut.negative("no-");

    REQUIRE_FALSE(sut.is_used());

    mark_negative_used(sut);
    CHECK(sut.is_used());
}

TEST_CASE_FIXTURE(argument_test_fixture, "count() should return 0 by default") {
    const auto sut = sut_type(arg_name_primary);
    CHECK_EQ(sut.count(), 0ull);
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "count() should return the number of times the argument's flag has been used "
    "[number of mark_used/mark_negative_used() function calls]"
) {
    auto sut = sut_type(arg_name_primary);
    REQUIRE_FALSE(sut.is_used());
    REQUIRE_EQ(sut.count(), 0ull);

    constexpr std::size_t count_pos = 5ull;
    for (std::size_t n = 0ull; n < count_pos; ++n)
        mark_used(sut);

    constexpr std::size_t count_neg = 3ull;
    for (std::size_t n = 0ull; n < count_neg; ++n)
        mark_negative_used(sut);

    CHECK_EQ(sut.count(), count_pos + count_neg);
}

TEST_CASE_FIXTURE(argument_test_fixture, "argument flag usage should trigger the on-flag actions") {
    auto sut = sut_type(arg_name_primary);

    const auto throw_action = []() { throw std::runtime_error("no reason"); };
    sut.actions<argon::action::on_flag>(throw_action);

    CHECK_THROWS_AS(mark_used(sut), std::runtime_error);
}

TEST_CASE_FIXTURE(
    argument_test_fixture, "argument flag usage with predefined store action should inject value"
) {
    auto sut = sut_type(arg_name_primary);
    sut.actions<argon::action::on_flag>(argon::action::store(arbitrary_value));

    mark_used(sut); // trigger the on-flag action

    REQUIRE(sut.has_parsed_values());
    CHECK_EQ(sut.value(), arbitrary_value);
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "argument flag usage with predefined store action should support variadic values"
) {
    auto sut = sut_type(arg_name_primary).nargs(argon::nargs::any());
    sut.actions<argon::action::on_flag>(argon::action::store(10, 20, 30));

    mark_used(sut); // trigger the on-flag action

    REQUIRE(sut.has_parsed_values());
    CHECK_EQ(sut.values(), (std::vector<sut_value_type>{10, 20, 30}));
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "argument flag usage with predefined store action should support range of values"
) {
    auto sut = sut_type(arg_name_primary).nargs(argon::nargs::any());

    std::vector<sut_value_type> test_values{10, 20, 30};
    sut.actions<argon::action::on_flag>(argon::action::store(test_values));

    mark_used(sut); // trigger the on-flag action

    REQUIRE(sut.has_parsed_values());
    CHECK_EQ(sut.values(), test_values);
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "argon::action::store as on-flag action should treat std::string as a single value"
) {
    argon::optional_argument<std::string> str_sut(arg_name_primary);
    str_sut.nargs(argon::nargs::any());

    std::string test_string = "hello world";
    str_sut.actions<argon::action::on_flag>(argon::action::store(test_string));

    mark_used(str_sut); // trigger the on-flag action

    REQUIRE(str_sut.has_parsed_values());
    CHECK_EQ(str_sut.values(), std::vector<std::string>{test_string});
}

TEST_CASE_FIXTURE(argument_test_fixture, "has_value() should return false by default") {
    const auto sut = sut_type(arg_name_primary);
    CHECK_FALSE(sut.has_value());
}

TEST_CASE_FIXTURE(argument_test_fixture, "has_value() should return true if value is set") {
    auto sut = sut_type(arg_name_primary);
    set_value(sut, arbitrary_value);
    CHECK(sut.has_value());
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "has_value() should return true if a default value is set and value() should return the "
    "default value"
) {
    auto sut = sut_type(arg_name_primary);
    sut.default_values(default_value);

    REQUIRE(sut.has_value());
    CHECK_EQ(sut.value(), default_value);
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "has_value() should return false if only the implicit value is set but the argument is not used"
) {
    auto sut = sut_type(arg_name_primary);
    sut.implicit_values(implicit_value);

    CHECK_FALSE(sut.has_value());
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "has_value() should return true if the implicit value is set and the agument is used"
) {
    auto sut = sut_type(arg_name_primary);
    sut.implicit_values(implicit_value);

    mark_used(sut);

    REQUIRE(sut.has_value());
    CHECK_EQ(sut.value(), implicit_value);
}

TEST_CASE_FIXTURE(argument_test_fixture, "has_parsed_values() should return false by default") {
    const auto sut = sut_type(arg_name_primary);
    CHECK_FALSE(sut.has_parsed_values());
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "has_parsed_values() should return false regardles of the "
    "default_values and implicit_values parameters"
) {
    auto sut = sut_type(arg_name_primary);

    SUBCASE("default_values") {
        sut.default_values(default_value);
        CHECK_FALSE(sut.has_parsed_values());
    }

    SUBCASE("implicit_values") {
        sut.implicit_values(implicit_value);
        CHECK_FALSE(sut.has_parsed_values());
    }

    SUBCASE("default_values and implicit_values") {
        sut.default_values(default_value);
        sut.implicit_values(implicit_value);
        CHECK_FALSE(sut.has_parsed_values());
    }
}

TEST_CASE_FIXTURE(argument_test_fixture, "has_parsed_values() should true if the value is set") {
    auto sut = sut_type(arg_name_primary);
    set_value(sut, arbitrary_value);
    CHECK(sut.has_parsed_values());
}

TEST_CASE_FIXTURE(argument_test_fixture, "has_predefined_values() should return false by default") {
    const auto sut = sut_type(arg_name_primary);
    CHECK_FALSE(sut.has_predefined_values());
}

TEST_CASE_FIXTURE(
    argument_test_fixture, "has_predefined_values() should return true if the default value is set"
) {
    auto sut = sut_type(arg_name_primary);
    sut.default_values(default_value);

    CHECK(sut.has_predefined_values());
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "has_predefined_values() should return false if the implicit value is set and the argument is "
    "not used"
) {
    auto sut = sut_type(arg_name_primary);
    sut.implicit_values(implicit_value);

    CHECK_FALSE(sut.has_predefined_values());
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "has_predefined_values() should return true if the implicit value is set and the argument is "
    "used"
) {
    auto sut = sut_type(arg_name_primary);
    sut.implicit_values(implicit_value);

    mark_used(sut);

    CHECK(sut.has_predefined_values());
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "has_predefined_values() should return true if both the default and implicit values are set"
) {
    auto sut = sut_type(arg_name_primary);
    sut.default_values(default_value);
    sut.implicit_values(implicit_value);

    CHECK(sut.has_predefined_values());
}

TEST_CASE_FIXTURE(
    argument_test_fixture, "value() should throw if the argument's value has not been set"
) {
    auto sut = sut_type(arg_name_primary);

    REQUIRE_FALSE(sut.has_value());
    CHECK_THROWS_AS(discard(sut.value()), std::logic_error);
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "value() should return the default value if one has been provided and argument is not used"
) {
    auto sut = sut_type(arg_name_primary);
    sut.default_values(arbitrary_value);

    REQUIRE(sut.has_value());
    CHECK_EQ(sut.value(), arbitrary_value);
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "value() should return the implicit value if one has been provided and argument is used"
) {
    auto sut = sut_type(arg_name_primary);
    sut.implicit_values(implicit_value);

    mark_used(sut);

    REQUIRE(sut.has_value());
    CHECK_EQ(sut.value(), implicit_value);
}

TEST_CASE_FIXTURE(
    argument_test_fixture, "value() should return the argument's value if it has been set"
) {
    auto sut = sut_type(arg_name_primary);
    sut.default_values(default_value);
    sut.implicit_values(implicit_value);
    set_value(sut, arbitrary_value);

    REQUIRE(sut.has_value());
    CHECK_EQ(sut.value(), arbitrary_value);
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "value_or() should return the fallback value if the argument's value has not been set"
) {
    auto sut = sut_type(arg_name_primary);
    constexpr sut_value_type fallback_value = 999;

    REQUIRE_FALSE(sut.has_value());
    CHECK_EQ(sut.value_or(fallback_value), fallback_value);
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "value_or() should return the default value if one has been provided and argument is not used"
) {
    auto sut = sut_type(arg_name_primary);
    sut.default_values(arbitrary_value);
    constexpr sut_value_type fallback_value = 999;

    REQUIRE(sut.has_value());
    CHECK_EQ(sut.value_or(fallback_value), arbitrary_value);
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "value_or() should return the implicit value if one has been provided and argument is used"
) {
    auto sut = sut_type(arg_name_primary);
    sut.implicit_values(implicit_value);
    constexpr sut_value_type fallback_value = 999;

    mark_used(sut);

    REQUIRE(sut.has_value());
    CHECK_EQ(sut.value_or(fallback_value), implicit_value);
}

TEST_CASE_FIXTURE(
    argument_test_fixture, "value_or() should return the argument's value if it has been set"
) {
    auto sut = sut_type(arg_name_primary);
    sut.default_values(default_value);
    sut.implicit_values(implicit_value);
    set_value(sut, arbitrary_value);
    constexpr sut_value_type fallback_value = 999;

    REQUIRE(sut.has_value());
    CHECK_EQ(sut.value_or(fallback_value), arbitrary_value);
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "set_value(any) should throw when the given string cannot be converted to an instance of "
    "value_type"
) {
    auto sut = sut_type(arg_name_primary);

    SUBCASE("given string is empty") {
        REQUIRE_THROWS_WITH_AS(
            set_value(sut, empty_str),
            invalid_value_msg(arg_name_primary, empty_str).c_str(),
            parsing_failure
        );
        CHECK_FALSE(sut.has_value());
    }
    SUBCASE("given string is non-convertible to value_type") {
        REQUIRE_THROWS_WITH_AS(
            set_value(sut, invalid_value_str),
            invalid_value_msg(arg_name_primary, invalid_value_str).c_str(),
            parsing_failure
        );
        CHECK_FALSE(sut.has_value());
    }
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "set_value(any) should throw when the choices set does not contain the parsed value"
) {
    auto sut = sut_type(arg_name_primary);
    sut.choices(choices);

    REQUIRE_THROWS_WITH_AS(
        set_value(sut, invalid_choice),
        doctest::Contains(invalid_choice_msg(arg_name_primary, as_string(invalid_choice)).c_str()),
        parsing_failure
    );

    CHECK_FALSE(sut.has_value());
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "set_value(any) should throw when adding the given value would result in exceeding the maximum "
    "number of values specified by nargs"
) {
    auto sut = sut_type(arg_name_primary).nargs(non_default_range);

    for (const auto value : choices)
        REQUIRE_NOTHROW(set_value(sut, value));

    REQUIRE_EQ(sut.values(), choices);

    CHECK_THROWS_WITH_AS(
        set_value(sut, arbitrary_value),
        parsing_failure::invalid_nvalues(arg_name_primary, std::weak_ordering::greater).what(),
        parsing_failure
    );
}

TEST_CASE_FIXTURE(
    argument_test_fixture, "set_value(any) should perform the specified value action"
) {
    auto sut = sut_type(arg_name_primary);

    SUBCASE("observe action") {
        const auto is_power_of_two = [](const sut_value_type n) {
            if (not ((n > 0) and (n & (n - 1)) == 0)) {
                throw std::runtime_error(std::format("Value `{}` is not a power of 2", n));
            }
        };

        sut.actions<argon::action::observe>(is_power_of_two);

        CHECK_THROWS_AS(set_value(sut, 3), std::runtime_error);

        sut_value_type valid_value = 16;
        REQUIRE_NOTHROW(set_value(sut, valid_value));
        CHECK_EQ(sut.value(), valid_value);
    }

    SUBCASE("transform action") {
        const auto double_action = [](const sut_value_type& value) { return 2 * value; };
        sut.actions<argon::action::transform>(double_action);

        set_value(sut, arbitrary_value);

        CHECK_EQ(sut.value(), double_action(arbitrary_value));
    }

    SUBCASE("modify action") {
        const auto double_action = [](sut_value_type& value) { value *= 2; };
        sut.actions<argon::action::modify>(double_action);

        auto test_value = arbitrary_value;

        set_value(sut, test_value);

        double_action(test_value);
        CHECK_EQ(sut.value(), test_value);
    }

    SUBCASE("multiple mixed actions execute sequentially") {
        sut_value_type capture = 0;
        sut.actions<argon::action::on_flag>([&]() { capture = 100; }
        ) // Only executes on flag parsing
            .actions<argon::action::observe>([&](const sut_value_type& v) { capture += v; })
            .actions<argon::action::transform>([](const sut_value_type& v) { return v + 1; })
            .actions<argon::action::modify>([](sut_value_type& v) { v *= 2; });

        // Trigger flag
        mark_used(sut);
        CHECK_EQ(capture, 100);

        // Trigger values
        // Initial value = 5
        // 1. observe: capture += 5  (capture becomes 105)
        // 2. transform: 5 + 1 = 6   (value becomes 6)
        // 3. modify: 6 * 2 = 12     (value becomes 12)
        set_value(sut, 5);

        CHECK_EQ(capture, 105);
        CHECK_EQ(sut.value(), 12);
    }
}

TEST_CASE_FIXTURE(
    argument_test_fixture, "nvalues_ordering() should return equivalent for default nargs (any)"
) {
    const auto sut = sut_type(arg_name_primary);
    CHECK(std::is_eq(nvalues_ordering(sut)));
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "nvalues_ordering() should return equivalent if a default value has been set"
) {
    auto sut = sut_type(arg_name_primary);
    sut.nargs(non_default_range);

    sut.default_values(default_value);

    CHECK(std::is_eq(nvalues_ordering(sut)));
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "nvalues_ordering() should return equivalent only when the number of values "
    "is in the specified range"
) {
    auto sut = sut_type(arg_name_primary);
    sut.nargs(non_default_range);

    REQUIRE(std::is_lt(nvalues_ordering(sut)));

    for (const auto value : choices) {
        REQUIRE_NOTHROW(set_value(sut, value));
        CHECK(std::is_eq(nvalues_ordering(sut)));
    }

    set_value_force(sut, invalid_choice);
    CHECK(std::is_gt(nvalues_ordering(sut)));
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "negative flag with discard_parsed should clear parsed values but preserve predefined values"
) {
    auto sut = sut_type(arg_name_primary);
    sut.default_values(default_value);
    sut.negative("no-", argon::action::discard_parsed);

    set_value(sut, arbitrary_value);
    REQUIRE(sut.has_parsed_values());
    REQUIRE_EQ(sut.value(), arbitrary_value);

    mark_negative_used(sut);

    CHECK_FALSE(sut.has_parsed_values());
    CHECK(sut.has_predefined_values()); // Predefined values are untouched
    CHECK_EQ(sut.value(), default_value);
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "negative flag with discard_all should clear parsed values and mask predefined values"
) {
    auto sut = sut_type(arg_name_primary);
    sut.default_values(default_value);
    sut.negative("no-", argon::action::discard_all);

    set_value(sut, arbitrary_value);
    REQUIRE(sut.has_parsed_values());
    REQUIRE(sut.has_predefined_values());

    mark_negative_used(sut);

    CHECK_FALSE(sut.has_parsed_values());
    CHECK_FALSE(sut.has_predefined_values()); // _ignore_predefined is now true
    CHECK_THROWS_AS(discard(sut.value()), std::logic_error);
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "reset() should clear parsed values, count, and restore masked predefined values"
) {
    auto sut = sut_type(arg_name_primary);
    sut.default_values(default_value);
    sut.negative("no-", argon::action::discard_all);

    mark_used(sut);
    set_value(sut, arbitrary_value);
    mark_negative_used(sut);

    REQUIRE_EQ(sut.count(), 2ull);
    REQUIRE_FALSE(sut.has_predefined_values());

    reset(sut); // This should flip _ignore_predefined back to false

    CHECK_EQ(sut.count(), 0ull);
    CHECK_FALSE(sut.has_parsed_values());
    CHECK(sut.has_predefined_values()); // Mask is lifted
    CHECK_EQ(sut.value(), default_value);
}

TEST_CASE_FIXTURE(
    argument_test_fixture,
    "negative boolean flags should automatically deduce and store the inverse of the default value"
) {
    SUBCASE("Default is true (e.g., add_flag<false>)") {
        argon::optional_argument<bool> sut(arg_name_primary);
        sut.default_values(true);
        sut.negative("no-"); // Automatically sets up discard_parsed + store(true)

        mark_negative_used(sut);

        REQUIRE(sut.has_parsed_values());
        CHECK_EQ(sut.value(), true);
    }

    SUBCASE("Default is false (e.g., add_flag<true>)") {
        argon::optional_argument<bool> sut(arg_name_primary);
        sut.default_values(false);
        sut.negative("no-"); // Automatically sets up discard_parsed + store(true)

        mark_negative_used(sut);

        REQUIRE(sut.has_parsed_values());
        CHECK_EQ(sut.value(), false);
    }

    SUBCASE("No default set (raw optional_argument<bool>)") {
        argon::optional_argument<bool> sut(arg_name_primary);
        sut.negative("no-"); // Fallback inverse is false

        mark_negative_used(sut);

        REQUIRE(sut.has_parsed_values());
        CHECK_EQ(sut.value(), false);
    }
}

TEST_SUITE_END(); // test_optional_argument
