// Copyright (c) 2023-2026 Jakub Musiał
// This file is part of the CPP-ARGON project (https://github.com/SpectraL519/cpp-argon).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file argon/action/types.hpp
/// @brief Defines the action category tags and state action specifiers.

#pragma once

#include "argon/traits.hpp"

#include <functional>
#include <vector>

namespace argon::action {

// --- Action Category Tags ---

/// @brief Action category tag for *value* actions (executed sequentially when parsing a value).
struct value {};

/// @brief Action category tag for *on-flag* state actions (executed when an optional flag is encountered).
struct on_flag {
    using type = std::function<void()>;
};

// --- Predefined State Action Specifiers ---

/// @brief Tag type used to discard an argument's parsed values.
struct discard_parsed_t {};

/// @brief Tag type used to discard an argument's parsed, default, and implicit values.
struct discard_all_t {};

/// @brief A type used to store/insert values for an argument by an argument action.
template <traits::c_argument_value_type V>
struct store_t {
    std::vector<V> values;
};

} // namespace argon::action
