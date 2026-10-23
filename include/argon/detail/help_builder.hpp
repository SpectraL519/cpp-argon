// Copyright (c) 2023-2026 Jakub Musiał
// This file is part of the CPP-ARGON project (https://github.com/SpectraL519/cpp-argon).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file argon/detail/help_builder.hpp
/// @brief Defines structures for creating and formatting help messages.

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

struct parameter_descriptor {
    std::string name;
    std::string value;
};

class help_builder {
public:
    help_builder(std::string name, std::string help = "")
    : name(std::move(name)), help(std::move(help)) {}

    void add_param(const std::string& param_name, const std::string& value) {
        this->params.emplace_back(param_name, value);
    }

    template <traits::c_writable T>
    void add_param(const std::string& param_name, const T& value) {
        std::ostringstream oss;
        oss << std::boolalpha << value;
        this->params.emplace_back(param_name, oss.str());
    }

    template <std::ranges::range R>
    requires(traits::c_writable<std::ranges::range_value_t<R>>)
    void add_range_param(
        const std::string& param_name,
        const R& range,
        const std::string_view delimiter = default_delimiter
    ) {
        this->params.emplace_back(param_name, util::join(range, delimiter));
    }

    // Builds a basic element description string in the format "<indent><arg-name> : <help-msg>"
    [[nodiscard]] std::string build_base(const format_config& fmt, const std::size_t align_to = 0)
        const {
        std::ostringstream oss;
        const std::size_t name_width = std::max(align_to, this->name.length());
        const std::size_t prefix_len = fmt.indent_width + name_width + 3; // + len(" : ")

        oss << std::string(fmt.indent_width, ' ') << std::left
            << std::setw(static_cast<int>(name_width)) << this->name;

        if (this->help.empty())
            return oss.str();

        oss << " : ";

        std::size_t text_width = 0;
        if (fmt.max_line_width > prefix_len)
            text_width = fmt.max_line_width - prefix_len;

        const auto lines = util::wrap_text(this->help, text_width);
        if (not lines.empty()) {
            oss << lines.front();
            const std::string padding(prefix_len, ' ');
            for (std::size_t i = 1; i < lines.size(); ++i)
                oss << '\n' << padding << lines[i];
        }

        return oss.str();
    }

    // Builds a full element description string, including parameters
    [[nodiscard]] std::string build(const format_config& fmt, const std::size_t align_to = 0) const {
        if (this->params.empty())
            return this->_build_compact(fmt, align_to);

        if (fmt.max_line_width > 0) {
            std::string single_line_str = this->_build_compact(fmt, align_to);

            std::size_t max_actual_len = 0;
            std::istringstream iss(single_line_str);
            std::string line;
            while (std::getline(iss, line))
                max_actual_len = std::max(max_actual_len, line.length());

            if (max_actual_len <= fmt.max_line_width)
                return single_line_str;
        }

        return this->_build_verbose(fmt, align_to);
    }

    std::string name;
    std::string help;
    std::vector<parameter_descriptor> params;

private:
    [[nodiscard]] std::string _build_compact(const format_config& fmt, const std::size_t align_to)
        const {
        std::ostringstream oss;
        oss << this->build_base(fmt, align_to);

        if (not this->params.empty()) {
            oss << " (" << util::join(this->params | std::views::transform([](const auto& param) {
                                          return std::format("{}: {}", param.name, param.value);
                                      }))
                << ")";
        }

        return oss.str();
    }

    [[nodiscard]] std::string _build_verbose(const format_config& fmt, const std::size_t align_to)
        const {
        std::ostringstream oss;
        oss << this->build_base(fmt, align_to);

        std::size_t max_param_name_len = 0ull;
        for (const auto& param : this->params)
            max_param_name_len = std::max(max_param_name_len, param.name.size());

        for (const auto& param : this->params) {
            oss << '\n'
                << std::string(fmt.indent_width * 2, ' ') << "- "
                << std::setw(static_cast<int>(max_param_name_len)) << std::left << param.name
                << " = " << param.value;
        }

        return oss.str();
    }

    static constexpr std::string_view default_delimiter = ", ";
};

} // namespace argon::detail
