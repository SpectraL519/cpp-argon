// Copyright (c) 2023-2026 Jakub Musiał
// This file is part of the CPP-ARGON project (https://github.com/SpectraL519/cpp-argon).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file argon/argument_group.hpp

#pragma once

#include "argon/argument_base.hpp"

#include <memory>

namespace argon {

/**
 * @brief Represents a group of arguments.
 *
 * Groups allow arguments to be organized under a dedicated section in the parser's help message.
 *
 * A group can be marked as:
 * - required: **at least one** argument from the group must be used in the command-line
 * - mutually exclusive: **at most one** argument from the group can be used in the command-line
 *
 * @note - This class is not intended to be constructed directly, but rather through the `add_group` method of @ref argon::argument_parser.
 * @note - User defined groups may contain only optional arguments (and flags).
 *
 * Example usage:
 * @code{.cpp}
 * argon::argument_parser parser("myprog");
 * auto& out_opts = parser.add_group("Output Options").mutually_exclusive();
 *
 * group.add_optional_argument(out_opts, "output", "o")
 *      .nargs(1)
 *      .help("Print output to the given file");
 *
 * group.add_optional_argument<argon::none_type>(out_opts, "print", "p")
 *      .help("Print output to the console");
 * @endcode
 * Here `out_opts` is a mutually exclusive group, so using both arguments at the same time would cause an error.
 */
// TODO: add attribute getters ???
class argument_group {
public:
    argument_group() = delete;

    /// @return The name of the argument group.
    [[nodiscard]] const std::string& name() const noexcept {
        return this->_name;
    }

    /**
     * @brief Set the help message for the argument group.
     * @param help_msg The help message to set.
     * @return Reference to the argument group instance.
     */
    argument_group& help(std::string_view help_msg) noexcept {
        this->_help_msg = help_msg;
        return *this;
    }

    /// @return The help message of the argument group.
    [[nodiscard]] const std::string& help() const noexcept {
        return this->_help_msg;
    }

    /**
     * @brief Set the `hidden` attribute of the group.
     *
     * - If set to true, the group will be hidden from the help output.
     * - Groups are NOT hidden by default.
     *
     * @param h The value to set for the attribute (default: true).
     * @return Reference to the group instance.
     */
    argument_group& hidden(const bool h = true) noexcept {
        this->_hidden = h;
        return *this;
    }

    /// @return `true` if the group is hidden from the help output, `false` otherwise.
    [[nodiscard]] bool is_hidden() const noexcept {
        return this->_hidden;
    }

    /**
     * @brief Set the `required` attribute of the group.
     *
     * - If set to true, the parser will require at least one argument from the group to be used in the command-line.
     * - If no arguments from the group are used, an exception will be thrown.
     * - Argument groups are NOT required by default.
     *
     * @param r The value to set for the attribute (default: true).
     * @return Reference to the group instance.
     */
    argument_group& required(const bool r = true) noexcept {
        this->_required = r;
        return *this;
    }

    /// @return `true` if the group is required, `false` otherwise.
    [[nodiscard]] bool is_required() const noexcept {
        return this->_required;
    }

    /**
     * @brief Set the `mutually_exclusive` attribute of the group.
     *
     * - If set to true, the parser will allow at most one argument from the group to be used in the command-line.
     * - If more than one argument from the group is used, an exception will be thrown.
     * - Argument groups are NOT mutually exclusive by default.
     *
     * @param me The value to set for the attribute (default: true).
     * @return Reference to the group instance.
     */
    argument_group& mutually_exclusive(const bool me = true) noexcept {
        this->_mutually_exclusive = me;
        return *this;
    }

    /// @return `true` if the group is mutually exclusive, `false` otherwise.
    [[nodiscard]] bool is_mutually_exclusive() const noexcept {
        return this->_mutually_exclusive;
    }

    /**
     * @brief Set the `prefix` attribute of the group.
     *
     * If set, each argument added to the group will have the given value prepended to its name.
     *
     * @param prefix The value to set for the attribute.
     * @return Reference to the group instance.
     */
    argument_group& with_prefix(std::string_view prefix) noexcept {
        this->_prefix = prefix;
        return *this;
    }

    /// @return The common prefix applied to arguments in this group.
    [[nodiscard]] const std::string& prefix() const noexcept {
        return this->_prefix;
    }

    /**
     * @brief Set the `suffix` attribute of the group.
     *
     * If set, each argument added to the group will have the given value appended to its name.
     *
     * @param suffix The value to set for the attribute.
     * @return Reference to the group instance.
     */
    argument_group& with_suffix(std::string_view suffix) noexcept {
        this->_suffix = suffix;
        return *this;
    }

    /// @return The common suffix applied to arguments in this group.
    [[nodiscard]] const std::string& suffix() const noexcept {
        return this->_suffix;
    }

    // --- argument value and state getters ---

    [[nodiscard]] bool is_used(std::string_view arg_base_name) const noexcept;
    [[nodiscard]] std::size_t count(std::string_view arg_base_name) const noexcept;
    [[nodiscard]] bool has_value(std::string_view arg_base_name) const noexcept;

    template <traits::c_argument_value_type T = std::string>
    [[nodiscard]] traits::argument_result_type<T> value(std::string_view arg_base_name) const;

    template <traits::c_argument_value_type T = std::string, std::convertible_to<T> U>
    [[nodiscard]] T value_or(std::string_view arg_base_name, U&& fallback_value) const;

    template <traits::c_argument_value_type T = std::string>
    [[nodiscard]] const std::vector<T>& values(std::string_view arg_base_name) const;

    friend class argument_parser;

private:
    using arg_ptr_t = std::shared_ptr<argument_base>; ///< The argument pointer type alias.
    using arg_ptr_vec_t = std::vector<arg_ptr_t>; ///< The argument pointer list type alias.

    /**
     * @brief Factory method to create an argument group.
     * @param parser The owning parser.
     * @param name Name of the group.
     */
    static std::unique_ptr<argument_group> create(argument_parser& parser, std::string_view name) {
        return std::unique_ptr<argument_group>(new argument_group(parser, name));
    }

    /// @brief Construct a new argument group with the given name.
    argument_group(argument_parser& parser, const std::string_view name)
    : _parser(&parser), _name(name) {}

    /// @brief Add a new argument to this group (called internally by parser).
    void _add_argument(arg_ptr_t arg) noexcept {
        this->_arguments.emplace_back(std::move(arg));
    }

    /// @brief Format the argument's base name by adding the group's prefix and suffix.
    [[nodiscard]] std::string _format_arg_name(std::string_view arg_base_name) const noexcept {
        return std::format("{}{}{}", this->_prefix, arg_base_name, this->_suffix);
    }

    // --- element references ---

    argument_parser* _parser; ///< Pointer to the owning parser.
    arg_ptr_vec_t _arguments = {}; ///< A list of arguments that belong to this group.

    // --- attributes ---

    std::string _name; ///< Name of the group (used in help output).
    std::string _prefix = ""; ///< Common argument flag prefix.
    std::string _suffix = ""; ///< Common argument flag suffix.
    std::string _help_msg = ""; ///< The argument's help message.

    bool _hidden : 1 = false; ///< The hidden attribute value (default: false).
    bool _required : 1 = false; ///< The required attribute value (default: false).
    bool _mutually_exclusive : 1 =
        false; ///< The mutually exclusive attribute value (default: false).
};

} // namespace argon
