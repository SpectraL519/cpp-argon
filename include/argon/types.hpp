// Copyright (c) 2023-2026 Jakub Musiał
// This file is part of the CPP-ARGON project (https://github.com/SpectraL519/cpp-argon).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file argon/types.hpp

#pragma once

#include <cstdint>
#include <format>
#include <iostream>

namespace argon {

/**
 * @brief A type representing the absence of a value.
 * This type is used for arguments that should not store any values
 * or as a fallback type for conditionally defined types.
 */
struct none_type {};

/// @brief Tag type used to enable dynamic program name deduction.
struct dynamic_name_t {
    explicit dynamic_name_t() = default;
};

/// @brief Tag value that enables dynamic deduction of the program name.
inline constexpr dynamic_name_t dynamic_name{};

/// @brief Configuration options for formatting the parser's help output.
struct format_config {
    std::uint8_t indent_width = 2u; ///< The number of spaces to prepend to sections.
    std::size_t max_line_width =
        120ull; ///< The maximum line width for text wrapping (0 disables wrapping).
};

} // namespace argon
