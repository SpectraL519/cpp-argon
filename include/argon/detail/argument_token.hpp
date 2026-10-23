// Copyright (c) 2023-2026 Jakub Musiał
// This file is part of the CPP-ARGON project (https://github.com/SpectraL519/cpp-argon).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file argon/detail/argument_token.hpp

#pragma once

#include "argon/argument_base.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace argon::detail {

struct argument_token {
    using arg_ptr_t = std::shared_ptr<argument_base>;
    using arg_ptr_vec_t = std::vector<arg_ptr_t>;

    enum class token_type : std::uint8_t {
        t_value,
        t_flag_primary,
        t_flag_secondary,
        t_flag_compound,
        t_flag_negative
    };
    using enum token_type;

    bool operator==(const argument_token& other) const noexcept {
        return this->type == other.type and this->value == other.value;
    }

    [[nodiscard]] bool is_flag_token() const noexcept {
        switch (this->type) {
        case t_flag_primary:
        case t_flag_secondary:
        case t_flag_compound:
        case t_flag_negative:
            return true;
        default:
            return false;
        }
    }

    // A flag token is valid if it was matched to at least one argument
    [[nodiscard]] bool is_valid_flag_token() const noexcept {
        return this->is_flag_token() and not this->args.empty();
    }

    token_type type;
    std::string value;
    arg_ptr_vec_t args = {};
};

} // namespace argon::detail
