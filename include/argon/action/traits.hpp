// Copyright (c) 2023-2026 Jakub Musiał
// This file is part of the CPP-ARGON project (https://github.com/SpectraL519/cpp-argon).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file argon/action/traits.hpp
/// @brief Defines action-related traits and concepts.

#pragma once

#include "argon/action/types.hpp"

#include <concepts>
#include <type_traits>

namespace argon::traits {

/// @brief Validates that `AC` is a valid action category tag (`action::value` or `action::on_flag`).
template <typename AC>
concept c_action_category = c_one_of<AC, action::value, action::on_flag>;

// --- Value Action Concepts ---

/// @brief Validates that `F` is a valid *observe* callable for value type `T` (`void(const T&)`).
/// @note Safely rejects callables taking non-const `T&`.
template <typename F, typename T>
concept c_observe_callable =
    c_argument_value_type<T> and std::invocable<std::remove_cvref_t<F>&, const T&>
    and std::same_as<std::invoke_result_t<std::remove_cvref_t<F>&, const T&>, void>;

/// @brief Validates that `F` is a valid *modify* callable for value type `T` (`void(T&)`).
/// @note Rejects callables taking `const T&` or `T` by value to prevent accidental no-op modifications.
template <typename F, typename T>
concept c_modify_callable =
    c_argument_value_type<T> and std::invocable<std::remove_cvref_t<F>&, T&>
    and (not std::invocable<std::remove_cvref_t<F>&, const T&>)
    and std::same_as<std::invoke_result_t<std::remove_cvref_t<F>&, T&>, void>;

/// @brief Validates that `F` is a valid value action callable (either observer or modifier) for value type `T`.
template <typename F, typename T>
concept c_value_action = c_observe_callable<F, T> or c_modify_callable<F, T>;

// --- State Action Concepts ---

/// @brief Concept validating if a type is a predefined state action tag.
template <typename T>
concept c_state_action_specifier =
    c_one_of<std::remove_cvref_t<T>, action::discard_parsed_t, action::discard_all_t>
    or c_instantiation_of<std::remove_cvref_t<T>, action::store_t>;

/// @brief Concept validating if a type is a valid state action (a specifier tag or a `void()` invocable callable).
template <typename F>
concept c_state_action =
    c_state_action_specifier<F>
    or (std::invocable<std::remove_cvref_t<F>&>
        and std::same_as<std::invoke_result_t<std::remove_cvref_t<F>&>, void>);

} // namespace argon::traits
