// Copyright (c) 2023-2026 Jakub Musiał
// This file is part of the CPP-ARGON project (https://github.com/SpectraL519/cpp-argon).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/**
 * @file argon/detail/help_builder.hpp
 * @brief Defines structures for creating and formatting help messages.
 */

#pragma once

#include "argon/argument_name.hpp"
#include "argon/traits.hpp"
#include "argon/util/string.hpp"

#include <algorithm>
#include <cstdint>
#include <format>
#include <iomanip>
#include <optional>
#include <sstream>
#include <vector>

namespace argon::detail {

/// @brief A structure used to represent an argument's parameter description.
struct parameter_descriptor {
    std::string name;
    std::string value;
};

/// @brief A help message builder class.
class help_builder {
public:
    /**
     * @param name The string representation of the argument's name.
     * @param help A help message string.
     */
    help_builder(const std::string& name, const std::string& help = "") : name(name), help(help) {}

    /**
     * @brief Adds a parameter descriptor with the given string value.
     * @param param_name The parameter's name.
     * @param value The parameter's value's string representation.
     */
    void add_param(const std::string& param_name, const std::string& value) {
        this->params.emplace_back(param_name, value);
    }

    /**
     * @brief Adds a parameter descriptor with the given value.
     * @tparam T The type of the parameter; must satisfy the @ref argon::traits::c_writable concept.
     * @param param_name The parameter's name.
     * @param value The parameter's value.
     */
    template <traits::c_writable T>
    void add_param(const std::string& param_name, const T& value) {
        std::ostringstream oss;
        oss << std::boolalpha << value;
        this->params.emplace_back(param_name, oss.str());
    }

    /**
     * @brief Adds a range parameter descriptor with the given value.
     * @tparam R The type of the parameter range. The value type of R must satisfy the @ref argon::traits::c_writable concept.
     * @param param_name The parameter's name.
     * @param range The parameter value range.
     * @param delimiter The delimiter used to separate the range values.
     */
    template <std::ranges::range R>
    requires(traits::c_writable<std::ranges::range_value_t<R>>)
    void add_range_param(
        const std::string& param_name,
        const R& range,
        const std::string_view delimiter = default_delimiter
    ) {
        this->params.emplace_back(param_name, util::join(range, delimiter));
    }

    /**
     * @param indent_width The indentation width.
     * @param align_to Optional minimum width for the argument name.
     * @param max_line_width Optional maximum line width for text align_textping.
     * @return A basic argument description string in the format "<indent><arg-name> : <help-msg>"
     */
    [[nodiscard]] std::string build_base(
        const uint8_t indent_width,
        const std::optional<std::size_t> align_to = std::nullopt,
        const std::optional<std::size_t> max_line_width = std::nullopt
    ) const {
        std::ostringstream oss;
        const std::size_t name_width = align_to.value_or(this->name.length());
        const std::size_t prefix_len = indent_width + name_width + 3; // + len(" : ")

        oss << std::string(indent_width, ' ') << std::left
            << std::setw(static_cast<int>(name_width)) << this->name;

        if (this->help.empty())
            return oss.str();

        oss << " : ";

        std::size_t text_width = 0;
        if (max_line_width.has_value() && max_line_width.value() > prefix_len)
            text_width = max_line_width.value() - prefix_len;

        const auto lines = util::wrap_text(this->help, text_width);
        if (not lines.empty()) {
            oss << lines.front();
            const std::string padding(prefix_len, ' ');
            for (std::size_t i = 1; i < lines.size(); ++i)
                oss << '\n' << padding << lines[i];
        }

        return oss.str();
    }

    /**
     * Generates a full string representation of the argument descriptor, optionally formatted
     * to fit within a specified maximum line width. The output includes the argument name,
     * help message (if present), and any added parameters.
     *
     * @param indent_width The number of spaces to insert before the argument name.
     * @param align_to Optional minimum width for the argument name.
     * @param max_line_width Optional maximum number of characters allowed for the one-line representation.
     * @return A formatted string describing the argument and its parameters.
     */
    [[nodiscard]] std::string build(
        const uint8_t indent_width,
        const std::optional<std::size_t> align_to = std::nullopt,
        const std::optional<std::size_t> max_line_width = std::nullopt
    ) const {
        if (this->params.empty())
            return this->_build_compact(indent_width, align_to, max_line_width);

        if (max_line_width.has_value()) {
            std::string single_line_str =
                this->_build_compact(indent_width, align_to, max_line_width);

            std::size_t max_actual_len = 0;
            std::istringstream iss(single_line_str);
            std::string line;
            while (std::getline(iss, line))
                max_actual_len = std::max(max_actual_len, line.length());

            if (max_actual_len <= max_line_width.value())
                return single_line_str;
        }

        return this->_build_verbose(indent_width, align_to, max_line_width);
    }

    std::string name;
    std::string help;
    std::vector<parameter_descriptor> params;

private:
    [[nodiscard]] std::string _build_compact(
        const uint8_t indent_width,
        const std::optional<std::size_t> align_to,
        const std::optional<std::size_t> max_line_width
    ) const {
        std::ostringstream oss;
        oss << this->build_base(indent_width, align_to, max_line_width);

        if (not this->params.empty()) {
            oss << " (" << util::join(this->params | std::views::transform([](const auto& param) {
                                          return std::format("{}: {}", param.name, param.value);
                                      }))
                << ")";
        }

        return oss.str();
    }

    [[nodiscard]] std::string _build_verbose(
        const uint8_t indent_width,
        const std::optional<std::size_t> align_to,
        const std::optional<std::size_t> max_line_width
    ) const {
        std::ostringstream oss;
        oss << this->build_base(indent_width, align_to, max_line_width);

        std::size_t max_param_name_len = 0ull;
        for (const auto& param : this->params)
            max_param_name_len = std::max(max_param_name_len, param.name.size());

        for (const auto& param : this->params) {
            oss << '\n'
                << std::string(indent_width * 2, ' ') << "- "
                << std::setw(static_cast<int>(max_param_name_len)) << std::left << param.name
                << " = " << param.value;
        }

        return oss.str();
    }

    static constexpr std::string_view default_delimiter = ", ";
};

} // namespace argon::detail
