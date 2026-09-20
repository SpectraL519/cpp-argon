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

TEST_CASE_FIXTURE(
    test_argument_parser_cfg, "parser's program version member should be nullopt by default"
) {
    CHECK_FALSE(get_program_version());
}

TEST_CASE_FIXTURE(
    test_argument_parser_cfg,
    "program_version() should throw if the version string contains whitespaces"
) {
    CHECK_THROWS_WITH_AS(
        sut.program_version("invalid version"),
        "The program version cannot contain whitespace characters!",
        invalid_configuration
    );
}

TEST_CASE_FIXTURE(test_argument_parser_cfg, "version() should set the program version member") {
    sut.program_version(test_version);
    auto stored_program_version = get_program_version();
    REQUIRE(stored_program_version);
    CHECK_EQ(stored_program_version.value(), test_version.str());

    sut.program_version(test_str_version);
    stored_program_version = get_program_version();
    REQUIRE(stored_program_version);
    CHECK_EQ(stored_program_version.value(), test_str_version);
}

TEST_CASE_FIXTURE(
    test_argument_parser_cfg, "parser's program description member should be nullopt by default"
) {
    const auto stored_program_description = get_program_description();
    CHECK_FALSE(stored_program_description);
}

TEST_CASE_FIXTURE(
    test_argument_parser_cfg, "program_description() should set the program description member"
) {
    sut.program_description(test_description);

    const auto stored_program_description = get_program_description();

    REQUIRE(stored_program_description);
    CHECK_EQ(stored_program_description.value(), test_description);
}

TEST_CASE_FIXTURE(
    test_argument_parser_cfg, "flag_char() should throw if the parser already has arguments"
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
    "flag_char() should throw if the given character is not a printable ASCII character"
) {
    CHECK_THROWS_WITH_AS(
        sut.flag_char('\n'),
        "The flag character must be a printable ASCII character!",
        invalid_configuration
    );
}

TEST_SUITE_END(); // test_argument_parser_cfg
