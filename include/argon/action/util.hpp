// Copyright (c) 2023-2026 Jakub Musiał
// This file is part of the CPP-ARGON project (https://github.com/SpectraL519/cpp-argon).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file argon/action/util.hpp
/// @brief Defines general action-related helper utility.

#pragma once

#include "argon/action/traits.hpp"

#include <functional>
#include <utility>
#include <variant>

namespace argon::action::util {

/// @brief State action callable type alias.
/// @ingroup util
using state_callable_type = typename action::on_flag::type;

/// @brief Variant type used to store different value action signatures.
template <traits::c_argument_value_type T>
using value_action_variant_type = std::variant<
    std::function<void(const T&)>, // observe
    std::function<void(T&)> // modify
    >;

/// @brief A visitor structure used to apply *value* actions.
/// @tparam T The argument's value type
/// @ingroup util
template <traits::c_argument_value_type T>
struct apply_visitor {
    using value_type = T;

    /// @brief The call operator overload for the *observe* action type.
    void operator()(const std::function<void(const value_type&)>& action) const {
        action(std::as_const(value));
    }

    /// @brief The call operator overload for the *modify* action type.
    void operator()(const std::function<void(value_type&)>& action) const {
        action(value);
    }

    value_type& value; ///< A reference to the argument's value for which the action will be applied.
};

} // namespace argon::action::util
