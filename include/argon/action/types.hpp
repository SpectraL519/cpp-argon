// Copyright (c) 2023-2026 Jakub Musiał
// This file is part of the CPP-ARGON project (https://github.com/SpectraL519/cpp-argon).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file argon/action/types.hpp
/// @brief Defies the action specifier types.

#pragma once

#include "argon/traits.hpp"

#include <functional>

namespace argon::action {

/// @brief An observing *value* action specifier.
///
/// Represents an argument action which observes the parsed value and
/// performs some logic on it without modifying it.
struct observe {
    template <traits::c_argument_value_type T>
    using type = std::function<void(const T&)>;
};

/// @brief A transformating *value* action specifier.
///
/// Represents an argument action which transforms the parsed value and
/// returns a new value with which the argument will be initialized.
struct transform {
    template <traits::c_argument_value_type T>
    using type = std::function<T(const T&)>;
};

/// @brief A modifying *value* action specifier.
///
/// Represents an argument action which modifies the value of an
/// already initialized argument.
struct modify {
    template <traits::c_argument_value_type T>
    using type = std::function<void(T&)>;
};

/// @brief An on-flag action specifier.
///
/// Represents an action associated with an argument that performs
/// some logic, independently of the parsed value.
///
/// @note An on-flag action is executed immediately after parsing the argument's flag and before parsing the following values.
struct on_flag {
    using type = std::function<void()>;
};

/// @brief Tag type used to reset an argument's parsed values.
struct reset_parsed_t {};

/// @brief Tag type used to clear an argument's parsed, default, and implicit values.
struct reset_all_t {};

/// @brief A type used to store/insert values for an argument by an argument action.
/// @todo Allow multiple values.
template <traits::c_argument_value_type V>
struct store_t {
    V value;
};

} // namespace argon::action
