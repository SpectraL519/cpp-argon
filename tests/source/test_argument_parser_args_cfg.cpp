#include "argument_parser_test_fixture.hpp"
#include "argument_test_fixture.hpp"
#include "doctest.h"
#include "utility.hpp"

using namespace argon_testing;
using argon::argument;
using argon::argument_parser;
using argon::default_argument;
using argon::invalid_configuration;
using argon::lookup_failure;

TEST_SUITE_BEGIN("test_argument_parser_args_cfg");

struct test_argument_parser_args_cfg : public argument_parser_test_fixture {
    const char flag_char = '-';

    const std::string primary_name_1 = "primary_name_1";
    const std::string secondary_name_1 = "s1";

    const std::string primary_name_2 = "primary_name_2";
    const std::string secondary_name_2 = "s2";

    const std::string invalid_name_empty = "";
    const std::string invalid_name_whitespace = "invalid name";
    const std::string invalid_name_flag_prefix = "-invalid";
    const std::string invalid_name_digit = "1invalid";
};

TEST_CASE_FIXTURE(
    test_argument_parser_args_cfg,
    "add_{positional,optional}_argument(primary) should throw if the passed argument name is "
    "invalid"
) {
    std::string primary_name, reason;

    SUBCASE("The name is empty") {
        primary_name = invalid_name_empty;
        reason = "An argument name cannot be empty.";
    }

    SUBCASE("The name contains whitespace characters") {
        primary_name = invalid_name_whitespace;
        reason = "An argument name cannot contain whitespaces.";
    }

    SUBCASE("The name begins with the flag prefix character") {
        primary_name = invalid_name_flag_prefix;
        reason = "An argument name cannot begin with a flag prefix character (-).";
    }

    SUBCASE("The name begins with a digit") {
        primary_name = invalid_name_digit;
        reason = "An argument name cannot begin with a digit.";
    }

    CAPTURE(primary_name);
    CAPTURE(reason);

    CHECK_THROWS_WITH_AS(
        sut.add_positional_argument(primary_name),
        invalid_configuration::invalid_argument_name(primary_name, reason).what(),
        invalid_configuration
    );

    CHECK_THROWS_WITH_AS(
        sut.add_optional_argument(primary_name),
        invalid_configuration::invalid_argument_name(primary_name, reason).what(),
        invalid_configuration
    );
}

TEST_CASE_FIXTURE(
    test_argument_parser_args_cfg,
    "add_{positional,optional}_argument(primary, secondary) should throw if the primary name is "
    "invalid"
) {
    std::string primary_name, reason;

    SUBCASE("The name is empty") {
        primary_name = invalid_name_empty;
        reason = "An argument name cannot be empty.";
    }

    SUBCASE("The name contains whitespace characters") {
        primary_name = invalid_name_whitespace;
        reason = "An argument name cannot contain whitespaces.";
    }

    SUBCASE("The name begins with the flag prefix character") {
        primary_name = invalid_name_flag_prefix;
        reason = "An argument name cannot begin with a flag prefix character (-).";
    }

    SUBCASE("The name begins with a digit") {
        primary_name = invalid_name_digit;
        reason = "An argument name cannot begin with a digit.";
    }

    CAPTURE(primary_name);
    CAPTURE(reason);

    CHECK_THROWS_WITH_AS(
        sut.add_positional_argument(primary_name),
        invalid_configuration::invalid_argument_name(primary_name, reason).what(),
        invalid_configuration
    );

    CHECK_THROWS_WITH_AS(
        sut.add_optional_argument(primary_name),
        invalid_configuration::invalid_argument_name(primary_name, reason).what(),
        invalid_configuration
    );
}

TEST_CASE_FIXTURE(
    test_argument_parser_args_cfg,
    "add_optional_argument(primary, secondary) should throw if the secondary name is "
    "invalid"
) {
    std::string secondary_name, reason;

    SUBCASE("The name is empty") {
        secondary_name = invalid_name_empty;
        reason = "An argument name cannot be empty.";
    }

    SUBCASE("The name contains whitespace characters") {
        secondary_name = invalid_name_whitespace;
        reason = "An argument name cannot contain whitespaces.";
    }

    SUBCASE("The name begins with the flag prefix character") {
        secondary_name = invalid_name_flag_prefix;
        reason = "An argument name cannot begin with a flag prefix character (-).";
    }

    SUBCASE("The name begins with a digit") {
        secondary_name = invalid_name_digit;
        reason = "An argument name cannot begin with a digit.";
    }

    CAPTURE(secondary_name);
    CAPTURE(reason);

    CHECK_THROWS_WITH_AS(
        sut.add_optional_argument(primary_name_1, secondary_name),
        invalid_configuration::invalid_argument_name(secondary_name, reason).what(),
        invalid_configuration
    );
}

TEST_CASE_FIXTURE(
    test_argument_parser_args_cfg,
    "add_positional_argument should throw when adding an argument with a previously used name"
) {
    sut.add_positional_argument(primary_name_1);

    // adding argument with a unique name
    CHECK_NOTHROW(sut.add_positional_argument(primary_name_2));

    // adding argument with a previously used name
    CHECK_THROWS_WITH_AS(
        sut.add_positional_argument(primary_name_1),
        invalid_configuration::argument_name_used(argument_name{primary_name_1}).what(),
        invalid_configuration
    );
}

TEST_CASE_FIXTURE(
    test_argument_parser_args_cfg,
    "add_optional_argument should throw when adding an argument with a previously used name"
) {
    sut.add_optional_argument(primary_name_1, secondary_name_1);

    SUBCASE("adding argument with a unique name") {
        CHECK_NOTHROW(sut.add_optional_argument(primary_name_2, secondary_name_2));
    }

    SUBCASE("adding argument with a previously used primary name") {
        CHECK_THROWS_WITH_AS(
            sut.add_optional_argument(primary_name_1, secondary_name_2),
            invalid_configuration::argument_name_used(
                argument_name{primary_name_1, secondary_name_2, flag_char}
            )
                .what(),
            invalid_configuration
        );
    }

    SUBCASE("adding argument with a previously used secondary name") {
        CHECK_THROWS_WITH_AS(
            sut.add_optional_argument(primary_name_2, secondary_name_1),
            invalid_configuration::argument_name_used(
                argument_name{primary_name_2, secondary_name_1, flag_char}
            )
                .what(),
            invalid_configuration
        );
    }
}

TEST_CASE_FIXTURE(
    test_argument_parser_args_cfg,
    "add_flag should return an optional argument reference with flag parameters"
) {
    const argument_test_fixture arg_fixture;

    SUBCASE("StoreImplicitly = true") {
        auto& argument = sut.add_flag(primary_name_1, secondary_name_1);

        REQUIRE(is_optional<bool>(argument));
        CHECK_FALSE(sut.value<bool>(primary_name_1));

        arg_fixture.mark_used(argument);
        CHECK(sut.value<bool>(primary_name_1));
    }

    SUBCASE("StoreImplicitly = false") {
        auto& argument = sut.add_flag<false>(primary_name_1, secondary_name_1);

        REQUIRE(is_optional<bool>(argument));
        CHECK(sut.value<bool>(primary_name_1));

        arg_fixture.mark_used(argument);
        CHECK_FALSE(sut.value<bool>(primary_name_1));
    }
}

TEST_CASE_FIXTURE(
    test_argument_parser_args_cfg,
    "add_flag should throw when adding and argument with a previously used name"
) {
    sut.add_flag(primary_name_1, secondary_name_1);

    SUBCASE("adding argument with a unique name") {
        CHECK_NOTHROW(sut.add_flag(primary_name_2, secondary_name_2));
    }

    SUBCASE("adding argument with a previously used primary name") {
        CHECK_THROWS_WITH_AS(
            sut.add_flag(primary_name_1, secondary_name_2),
            invalid_configuration::argument_name_used(
                argument_name{primary_name_1, secondary_name_2, flag_char}
            )
                .what(),
            invalid_configuration
        );
    }

    SUBCASE("adding argument with a previously used secondary name") {
        CHECK_THROWS_WITH_AS(
            sut.add_flag(primary_name_2, secondary_name_1),
            invalid_configuration::argument_name_used(
                argument_name{primary_name_2, secondary_name_1, flag_char}
            )
                .what(),
            invalid_configuration
        );
    }
}

TEST_CASE_FIXTURE(
    test_argument_parser_args_cfg, "default_arguments should add the specified positional arguments"
) {
    sut.default_arguments({default_argument::p_input, default_argument::p_output});

    const auto input_arg = get_argument("input");
    REQUIRE(input_arg);
    CHECK(is_positional<std::string>(*input_arg));

    const auto output_arg = get_argument("output");
    REQUIRE(output_arg);
    CHECK(is_positional<std::string>(*output_arg));
}

TEST_CASE_FIXTURE(
    test_argument_parser_args_cfg, "default_arguments should add the specified optional arguments"
) {
    sut.default_arguments(
        default_argument::o_help, default_argument::o_input, default_argument::o_output
    );

    std::string help_flag;
    std::string input_flag;
    std::string output_flag;

    SUBCASE("using primary flags") {
        help_flag = "help";
        input_flag = "input";
        output_flag = "output";
    }

    SUBCASE("using secondary flags") {
        help_flag = "h";
        input_flag = "i";
        output_flag = "o";
    }

    CAPTURE(help_flag);
    CAPTURE(input_flag);
    CAPTURE(output_flag);

    const auto help_arg = get_argument(help_flag);
    REQUIRE(help_arg);
    CHECK(is_optional<argon::none_type>(*help_arg));

    const auto input_arg = get_argument(input_flag);
    REQUIRE(input_arg);
    CHECK(is_optional<std::string>(*input_arg));


    const auto output_arg = get_argument(output_flag);
    REQUIRE(output_arg);
    CHECK(is_optional<std::string>(*output_arg));
}

TEST_CASE_FIXTURE(
    test_argument_parser_args_cfg,
    "argument adding functions should throw if a group does not belong to the parser"
) {
    argument_parser different_parser("different-program");

    const std::string group_name = "Group From a Different Parser";
    auto& group = different_parser.add_group(group_name);

    const std::string expected_err_msg =
        std::format("An argument group '{}' does not belong to the given parser.", group_name);

    CHECK_THROWS_WITH_AS(
        sut.add_optional_argument(group, primary_name_1), expected_err_msg.c_str(), std::logic_error
    );

    CHECK_THROWS_WITH_AS(
        sut.add_optional_argument(group, primary_name_1), expected_err_msg.c_str(), std::logic_error
    );

    CHECK_THROWS_WITH_AS(
        sut.add_optional_argument(group, secondary_name_1, argon::n_secondary),
        expected_err_msg.c_str(),
        std::logic_error
    );

    CHECK_THROWS_WITH_AS(
        sut.add_optional_argument(group, primary_name_1, secondary_name_1),
        expected_err_msg.c_str(),
        std::logic_error
    );

    CHECK_THROWS_WITH_AS(
        sut.add_flag(group, primary_name_1), expected_err_msg.c_str(), std::logic_error
    );

    CHECK_THROWS_WITH_AS(
        sut.add_flag(group, secondary_name_1, argon::n_secondary),
        expected_err_msg.c_str(),
        std::logic_error
    );

    CHECK_THROWS_WITH_AS(
        sut.add_flag(group, primary_name_1, secondary_name_1),
        expected_err_msg.c_str(),
        std::logic_error
    );
}

TEST_CASE_FIXTURE(
    test_argument_parser_args_cfg,
    "argument adding functions should properly apply the argument group's name modifiers"
) {
    const auto group_name = "An Argument Group";
    const auto group_pre = "pre-";
    const auto group_suf = "-suf";

    auto& group = sut.add_group(group_name).with_prefix(group_pre).with_suffix(group_suf);

    sut.add_positional_argument(group, "positional");
    sut.add_optional_argument(group, "optional");
    sut.add_flag(group, "flag");

    const auto expected_err_msg = [](const auto& arg_name) -> std::string {
        return lookup_failure::argument_not_found(arg_name).what();
    };

    CHECK_THROWS_WITH_AS(
        discard(sut.value("positional")), expected_err_msg("positional").c_str(), lookup_failure
    );
    CHECK_THROWS_WITH_AS(
        discard(sut.value("optional")), expected_err_msg("optional").c_str(), lookup_failure
    );
    CHECK_THROWS_WITH_AS(
        discard(sut.value<bool>("flag")), expected_err_msg("flag").c_str(), lookup_failure
    );

    CHECK_FALSE(sut.has_value("pre-positional-suf"));
    CHECK_FALSE(sut.has_value("pre-optional-suf"));
    CHECK_FALSE(sut.value<bool>("pre-flag-suf"));
}

TEST_CASE_FIXTURE(
    test_argument_parser_args_cfg,
    "add_subparser should throw if a subparser with the given name already exists"
) {
    constexpr std::string_view subparser_name = "subprogram";

    sut.add_subparser(subparser_name);

    CHECK_THROWS_WITH_AS(
        sut.add_subparser(subparser_name),
        std::format(
            "A subparser with the given name ({}) already exists in parser '{}'",
            subparser_name,
            sut.name()
        )
            .c_str(),
        std::logic_error
    );
}

// argument getters

TEST_CASE_FIXTURE(
    test_argument_parser_args_cfg,
    "argument() getter should throw if there is no argument with the given name"
) {
    CHECK_THROWS_AS(discard(sut.argument("nonexistent")), lookup_failure);
}

TEST_CASE_FIXTURE(
    test_argument_parser_args_cfg,
    "argument() getter should return the correct argument base reference"
) {
    sut.add_positional_argument("pos_arg");
    sut.add_optional_argument("opt_arg", "o");

    CHECK_NOTHROW(discard(sut.argument("pos_arg")));
    CHECK_EQ(sut.argument("pos_arg").name().primary(), "pos_arg");
    CHECK(sut.argument("pos_arg").is_positional());

    CHECK_NOTHROW(discard(sut.argument("opt_arg")));
    CHECK_EQ(sut.argument("opt_arg").name().primary(), "opt_arg");
    CHECK(sut.argument("opt_arg").is_optional());

    // Should also be retrievable by the secondary name
    CHECK_NOTHROW(discard(sut.argument("o")));
    CHECK_EQ(sut.argument("o").name().secondary(), "o");
}

// group getters

TEST_CASE_FIXTURE(
    test_argument_parser_args_cfg,
    "group() getter should throw if there is no group with the given name"
) {
    CHECK_THROWS_AS(discard(sut.group("Nonexistent Group")), lookup_failure);

    const auto& const_sut = sut;
    CHECK_THROWS_AS(discard(const_sut.group("Nonexistent Group")), lookup_failure);
}

TEST_CASE_FIXTURE(
    test_argument_parser_args_cfg,
    "group() getter should return the correct argument group reference"
) {
    const std::string group_name = "My Group";
    sut.add_group(group_name);

    // Non-const getter
    CHECK_NOTHROW(discard(sut.group(group_name)));

    // Const getter
    const auto& const_sut = sut;
    CHECK_NOTHROW(discard(const_sut.group(group_name)));
}

TEST_CASE_FIXTURE(
    test_argument_parser_args_cfg,
    "argument_group attribute getters should return correct default values"
) {
    const std::string group_name = "Test Group";
    const auto& group = sut.add_group(group_name);

    CHECK_EQ(group.name(), group_name);
    CHECK(group.description().empty());
    CHECK_FALSE(group.is_hidden());
    CHECK_FALSE(group.is_required());
    CHECK_FALSE(group.is_mutually_exclusive());
    CHECK(group.prefix().empty());
    CHECK(group.suffix().empty());
}

TEST_CASE_FIXTURE(
    test_argument_parser_args_cfg,
    "argument_group setters should correctly update the group attributes"
) {
    const std::string group_name = "Test Group";
    auto& group = sut.add_group(group_name);

    const std::string description = "This is a test group description.";
    const std::string prefix = "pre-";
    const std::string suffix = "-suf";

    // Set all attributes
    group.description(description)
        .hidden()
        .required()
        .mutually_exclusive()
        .with_prefix(prefix)
        .with_suffix(suffix);

    // Verify getters reflect the updated state
    CHECK_EQ(group.description(), description);
    CHECK(group.is_hidden());
    CHECK(group.is_required());
    CHECK(group.is_mutually_exclusive());
    CHECK_EQ(group.prefix(), prefix);
    CHECK_EQ(group.suffix(), suffix);

    // Verify boolean setters can explicitly toggle flags off
    group.hidden(false).required(false).mutually_exclusive(false);

    CHECK_FALSE(group.is_hidden());
    CHECK_FALSE(group.is_required());
    CHECK_FALSE(group.is_mutually_exclusive());
}

// subparser getters

TEST_CASE_FIXTURE(
    test_argument_parser_args_cfg,
    "subparser() getter should throw if there is no subparser with the given name"
) {
    CHECK_THROWS_AS(discard(sut.subparser("nonexistent")), lookup_failure);

    const auto& const_sut = sut;
    CHECK_THROWS_AS(discard(const_sut.subparser("nonexistent")), lookup_failure);
}

TEST_CASE_FIXTURE(
    test_argument_parser_args_cfg,
    "subparser() getter should return the correct subparser reference"
) {
    const std::string sub_name = "my_sub";
    sut.add_subparser(sub_name);

    // Non-const getter
    CHECK_NOTHROW(discard(sut.subparser(sub_name)));
    CHECK_EQ(sut.subparser(sub_name).name(), sub_name);

    // Const getter
    const auto& const_sut = sut;
    CHECK_NOTHROW(discard(const_sut.subparser(sub_name)));
    CHECK_EQ(const_sut.subparser(sub_name).name(), sub_name);
}

TEST_SUITE_END(); // test_argument_parser_args_cfg
