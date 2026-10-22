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
/// @tparam AC The action category type.
/// @ingroup traits
template <typename AC>
concept c_action_category = c_one_of<AC, action::value, action::on_flag>;

/// @brief Validates that `AS` is a *value* action specifier instantiation (`observe_t`, `transform_t`, or `modify_t`).
/// @tparam AS The action specifier type.
/// @ingroup traits
template <typename AS>
concept c_value_action_specifier =
    c_instantiation_of<std::remove_cvref_t<AS>, action::observe_t>
    or c_instantiation_of<std::remove_cvref_t<AS>, action::transform_t>
    or c_instantiation_of<std::remove_cvref_t<AS>, action::modify_t>;

/// @brief Validates that `F` is a valid *observe* callable for value type `T` (`void(const T&)`).
/// @note Rejects callables taking non-const `T&`.
template <typename F, typename T>
concept c_observe_callable_for =
    c_argument_value_type<T> and std::invocable<F&, const T&>
    and std::same_as<std::invoke_result_t<F&, const T&>, void>;

/// @brief Validates that `F` is a valid *transform* callable for value type `T` (`T(const T&)`).
/// @note Rejects callables taking non-const `T&`.
template <typename F, typename T>
concept c_transform_callable_for =
    c_argument_value_type<T> and std::invocable<F&, const T&>
    and std::convertible_to<std::invoke_result_t<F&, const T&>, T>;

/// @brief Validates that `F` is a valid *modify* callable for value type `T` (`void(T&)`).
/// @note Rejects callables taking `const T&` or `T` by value to prevent accidental no-op modifications.
template <typename F, typename T>
concept c_modify_callable_for =
    c_argument_value_type<T> and std::invocable<F&, T&> and (not std::invocable<F&, const T&>)
    and std::same_as<std::invoke_result_t<F&, T&>, void>;

/// @brief Validates that `A` is an `action::observe_t<F>` with a valid observe signature for `T`.
template <typename A, typename T>
concept c_observe_action_for =
    c_instantiation_of<std::remove_cvref_t<A>, action::observe_t>
    and requires(std::remove_cvref_t<A> a
    ) { requires c_observe_callable_for<decltype(a.callable), T>; };

/// @brief Validates that `A` is an `action::transform_t<F>` with a valid transform signature for `T`.
template <typename A, typename T>
concept c_transform_action_for =
    c_instantiation_of<std::remove_cvref_t<A>, action::transform_t>
    and requires(std::remove_cvref_t<A> a
    ) { requires c_transform_callable_for<decltype(a.callable), T>; };

/// @brief Validates that `A` is an `action::modify_t<F>` with a valid modify signature for `T`.
template <typename A, typename T>
concept c_modify_action_for =
    c_instantiation_of<std::remove_cvref_t<A>, action::modify_t>
    and requires(std::remove_cvref_t<A> a
    ) { requires c_modify_callable_for<decltype(a.callable), T>; };

/// @brief Validates that `A` is a valid value action specifier (`observe_t`, `transform_t`, or `modify_t`) for value type `T`.
/// @tparam A The action specifier type.
/// @tparam T The argument's value type.
/// @ingroup traits
template <typename A, typename T>
concept c_value_action_for =
    c_observe_action_for<A, T> or c_transform_action_for<A, T> or c_modify_action_for<A, T>;

/// @brief Concept validating if a type is a predefined state action tag.
template <typename T>
concept c_state_action_specifier =
    c_one_of<std::remove_cvref_t<T>, action::discard_parsed_t, action::discard_all_t>
    or c_instantiation_of<std::remove_cvref_t<T>, action::store_t>;

/// @brief Concept validating if a type is a valid state action (a specifier tag or a `void()` invocable callable).
template <typename F>
concept c_state_action =
    c_state_action_specifier<F>
    or (std::invocable<F&> and std::same_as<std::invoke_result_t<F&>, void>);

} // namespace argon::traits
