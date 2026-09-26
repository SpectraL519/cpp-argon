#include "argument_parser_test_fixture.hpp"
#include "doctest.h"

using namespace argon_testing;

using argon::argument_parser;
using argon::invalid_configuration;

TEST_SUITE_BEGIN("test_argument_parser_cfg");

struct test_argument_parser_cfg : public argument_parser_test_fixture {
    const std::string test_description = "test program description";
    const argon::version test_version{1u, 2u, 3u};
    const std::string test_str_version = "alpha";
};

TEST_CASE("argument_parser() should throw if the name is empty") {
    CHECK_THROWS_WITH_AS(
        argument_parser(""), "The program name cannot be empty!", invalid_configuration
    );
}

TEST_CASE("argument_parser() should throw if the name contains whitespaces") {
    CHECK_THROWS_WITH_AS(
        argument_parser("invalid name"),
        "The program name cannot contain whitespace characters!",
        invalid_configuration
    );
}

TEST_CASE_FIXTURE(
    test_argument_parser_cfg, "argument_parser() should set the name and program name members"
) {
    CHECK_EQ(sut.name(), program_name);
    CHECK_EQ(sut.program_name(), program_name);
}

TEST_CASE_FIXTURE(
    test_argument_parser_cfg,
    "subparser's program name should be a concatenation of the parent parser's name and its own "
    "name"
) {
    constexpr std::string_view subparser_name = "subprogram";

    auto& subparser = sut.add_subparser(subparser_name);
    CHECK_EQ(subparser.name(), subparser_name);
    CHECK_EQ(subparser.program_name(), std::format("{} {}", sut.name(), subparser_name));
}

// --- version ---

TEST_CASE_FIXTURE(test_argument_parser_cfg, "version() getter should return nullopt by default") {
    CHECK_FALSE(sut.version());
}

TEST_CASE_FIXTURE(
    test_argument_parser_cfg,
    "version() setter should throw if the version string contains whitespaces"
) {
    CHECK_THROWS_WITH_AS(
        sut.version("invalid version"),
        "The program version cannot contain whitespace characters!",
        invalid_configuration
    );
}

TEST_CASE_FIXTURE(test_argument_parser_cfg, "version() setter should update the version member") {
    sut.version(test_version);
    auto stored_version = sut.version();
    REQUIRE(stored_version);
    CHECK_EQ(stored_version.value(), test_version.str());

    sut.version(test_str_version);
    stored_version = sut.version();
    REQUIRE(stored_version);
    CHECK_EQ(stored_version.value(), test_str_version);
}

// --- description ---

TEST_CASE_FIXTURE(
    test_argument_parser_cfg, "description() getter should return an empty string by default"
) {
    CHECK(sut.description().empty());
}

TEST_CASE_FIXTURE(
    test_argument_parser_cfg, "description() setter should update the description member"
) {
    sut.description(test_description);

    const auto stored_description = sut.description();

    REQUIRE_FALSE(stored_description.empty());
    CHECK_EQ(stored_description, test_description);
}

// --- is_verbose ---

TEST_CASE_FIXTURE(test_argument_parser_cfg, "is_verbose() should return false by default") {
    CHECK_FALSE(sut.is_verbose());
}

TEST_CASE_FIXTURE(test_argument_parser_cfg, "verbose() setter should update the verbosity mode") {
    sut.verbose();
    CHECK(sut.is_verbose());

    sut.verbose(false);
    CHECK_FALSE(sut.is_verbose());
}

// --- unknown_arguments_policy ---

TEST_CASE_FIXTURE(
    test_argument_parser_cfg, "unknown_arguments_policy() getter should return fail by default"
) {
    CHECK_EQ(sut.unknown_arguments_policy(), argon::unknown_policy::fail);
}

TEST_CASE_FIXTURE(
    test_argument_parser_cfg, "unknown_arguments_policy() setter should update the policy member"
) {
    sut.unknown_arguments_policy(argon::unknown_policy::warn);
    CHECK_EQ(sut.unknown_arguments_policy(), argon::unknown_policy::warn);

    sut.unknown_arguments_policy(argon::unknown_policy::ignore);
    CHECK_EQ(sut.unknown_arguments_policy(), argon::unknown_policy::ignore);

    sut.unknown_arguments_policy(argon::unknown_policy::as_values);
    CHECK_EQ(sut.unknown_arguments_policy(), argon::unknown_policy::as_values);
}

// --- flag_char ---

TEST_CASE_FIXTURE(test_argument_parser_cfg, "flag_char() getter should return '-' by default") {
    CHECK_EQ(sut.flag_char(), '-');
}

TEST_CASE_FIXTURE(
    test_argument_parser_cfg, "flag_char() setter should throw if the parser already has arguments"
) {
    sut.add_positional_argument("arg1");
    sut.add_optional_argument("arg2");

    CHECK_THROWS_WITH_AS(
        sut.flag_char('/'),
        "The flag character must be set before adding any arguments!",
        invalid_configuration
    );
}

TEST_CASE_FIXTURE(
    test_argument_parser_cfg,
    "flag_char() setter should throw if the given character is not a printable ASCII character"
) {
    CHECK_THROWS_WITH_AS(
        sut.flag_char('\n'),
        "The flag character must be a printable ASCII character!",
        invalid_configuration
    );
}

TEST_CASE_FIXTURE(
    test_argument_parser_cfg,
    "flag_char() setter should throw if the given character is the same as the assignment character"
) {
    CHECK_THROWS_WITH_AS(
        sut.flag_char('='),
        "The flag character cannot be the same as the assignment character!",
        invalid_configuration
    );
}

TEST_CASE_FIXTURE(
    test_argument_parser_cfg, "flag_char() setter should update the flag character member"
) {
    sut.flag_char('/');
    CHECK_EQ(sut.flag_char(), '/');
}

// --- assign_char ---

TEST_CASE_FIXTURE(test_argument_parser_cfg, "assign_char() getter should return '='") {
    CHECK_EQ(argon::argument_parser::assign_char(), '=');
}

TEST_SUITE_END(); // test_argument_parser_cfg
