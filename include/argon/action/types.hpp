// Copyright (c) 2023-2026 Jakub Musiał
// This file is part of the CPP-ARGON project (https://github.com/SpectraL519/cpp-argon).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file argon/action/types.hpp
/// @brief Defines the action category tags and action specifiers.

#pragma once

#include "argon/traits.hpp"

#include <functional>
#include <type_traits>
#include <utility>
#include <vector>

namespace argon::action {

// --- Action Category Tags ---

/// @brief Action category tag for *value* actions (executed when parsing a value).
struct value {};

/// @brief Action category tag for *on-flag* state actions (executed when parsing an optional flag).
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

// --- Value Action Specifiers ---

/// @brief An observing *value* action specifier.
///
/// Represents an argument action which observes the parsed value (`const T&`)
/// and performs some logic on it without modifying it.
template <typename F = void>
struct observe_t {
    template <traits::c_argument_value_type T>
    using type = std::function<void(const T&)>;

    F callable;
};

template <>
struct observe_t<void> {
    template <traits::c_argument_value_type T>
    using type = std::function<void(const T&)>;
};

/// @brief Factory function creating an *observe* value action specifier.
template <typename F>
requires(not std::is_void_v<std::remove_cvref_t<F>>)
[[nodiscard]] constexpr observe_t<std::remove_cvref_t<F>> observe(F&& callable
) noexcept(std::is_nothrow_constructible_v<std::remove_cvref_t<F>, F>) {
    return {std::forward<F>(callable)};
}

/// @brief A transforming *value* action specifier.
///
/// Represents an argument action which takes the parsed value (`const T&`) and
/// returns a new `T` value with which the argument will be initialized.
template <typename F = void>
struct transform_t {
    template <traits::c_argument_value_type T>
    using type = std::function<T(const T&)>;

    F callable;
};

template <>
struct transform_t<void> {
    template <traits::c_argument_value_type T>
    using type = std::function<T(const T&)>;
};

/// @brief Factory function creating a *transform* value action specifier.
template <typename F>
requires(not std::is_void_v<std::remove_cvref_t<F>>)
[[nodiscard]] constexpr transform_t<std::remove_cvref_t<F>> transform(F&& callable
) noexcept(std::is_nothrow_constructible_v<std::remove_cvref_t<F>, F>) {
    return {std::forward<F>(callable)};
}

/// @brief A modifying *value* action specifier.
///
/// Represents an argument action which modifies the value (`T&`) of an
/// already initialized argument in-place.
template <typename F = void>
struct modify_t {
    template <traits::c_argument_value_type T>
    using type = std::function<void(T&)>;

    F callable;
};

template <>
struct modify_t<void> {
    template <traits::c_argument_value_type T>
    using type = std::function<void(T&)>;
};

/// @brief Factory function creating a *modify* value action specifier.
template <typename F>
requires(not std::is_void_v<std::remove_cvref_t<F>>)
[[nodiscard]] constexpr modify_t<std::remove_cvref_t<F>> modify(F&& callable
) noexcept(std::is_nothrow_constructible_v<std::remove_cvref_t<F>, F>) {
    return {std::forward<F>(callable)};
}

} // namespace argon::action
