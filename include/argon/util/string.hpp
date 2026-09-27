// Copyright (c) 2023-2026 Jakub Musiał
// This file is part of the CPP-ARGON project (https://github.com/SpectraL519/cpp-argon).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/**
 * @file argon/util/string.hpp
 * @brief Provides common string utility functions.
 */

#pragma once

#include "argon/traits.hpp"

#include <algorithm>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace argon::util {

/**
 * @brief Converts a value to `std::string`.
 * @tparam T The value type (must satisfy the @ref argon::traits::c_writable concept).
 * @param value The value to convert.
 * @ingroup util
 */
template <traits::c_writable T>
[[nodiscard]] std::string as_string(T&& value) noexcept {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

/// @brief Checks whether a string contains any whitespace characters.
[[nodiscard]] inline bool contains_whitespaces(std::string_view str) noexcept {
    return std::ranges::any_of(str, [](unsigned char c) { return std::isspace(c); });
}

/**
 * @brief Joins elements of a range into a single string with a delimiter.
 * @tparam R The type of the value range. The value type of R must satisfy the @ref c_writable concept.
 * @param range The input range to join.
 * @param delimiter The separator string to insert between elements.
 * @return A single string with all elements joined by the delimiter.
 * @note This function applies the `std::boolalpha` to the string stream used within it.
 * @ingroup util
 */
template <std::ranges::range R>
requires(traits::c_writable<std::ranges::range_value_t<R>>)
[[nodiscard]] std::string join(R&& range, const std::string_view delimiter = ", ") {
    std::ostringstream oss;
    oss << std::boolalpha;

    auto it = std::ranges::begin(range);
    const auto end = std::ranges::end(range);
    if (it != end) {
        oss << *it;
        ++it;
        for (; it != end; ++it)
            oss << delimiter << *it;
    }

    return oss.str();
}

// TODO: add doc
[[nodiscard]] inline std::vector<std::string> wrap_text(
    const std::string_view text, const std::size_t max_width
) {
    std::vector<std::string> lines;
    if (text.empty())
        return lines;

    std::istringstream iss{std::string(text)};
    std::string manual_line;

    while (std::getline(iss, manual_line)) {
        if (max_width == 0 or manual_line.length() <= max_width) {
            lines.push_back(manual_line);
            continue;
        }

        std::string current_line;
        std::istringstream word_stream(manual_line);
        std::string word;

        while (word_stream >> word) {
            if (current_line.empty()) {
                current_line = word;
            }
            else if (current_line.length() + 1 + word.length() <= max_width) {
                current_line += " " + word;
            }
            else {
                lines.push_back(current_line);
                current_line = word;
            }
        }

        if (not current_line.empty())
            lines.push_back(current_line);
    }

    return lines;
}

/**
 * @brief Formats a block of text, wrapping it to the specified max width and preserving the indent.
 * @param text The text to format.
 * @param indent_width The number of spaces to prepend to each line.
 * @param max_line_width The maximum total width of a line (including indent). 0 disables wrapping.
 * @return The formatted string.
 */
[[nodiscard]] inline std::string align_text(
    const std::string_view text,
    const std::uint8_t indent_width,
    const std::size_t max_line_width = 0
) {
    if (text.empty())
        return "";

    std::size_t text_width = 0;
    if (max_line_width > indent_width)
        text_width = max_line_width - indent_width;

    const auto lines = wrap_text(text, text_width);
    if (lines.empty())
        return "";

    std::ostringstream oss;
    const std::string padding(indent_width, ' ');

    oss << padding << lines.front();
    for (std::size_t i = 1; i < lines.size(); ++i)
        oss << '\n' << padding << lines[i];

    return oss.str();
}

} // namespace argon::util
