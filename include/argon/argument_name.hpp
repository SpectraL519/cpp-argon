// Copyright (c) 2023-2026 Jakub Musiał
// This file is part of the CPP-ARGON project (https://github.com/SpectraL519/cpp-argon).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file argon/argument_name.hpp

#pragma once

#include <cstdint>
#include <format>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace argon {

/// @brief Class holding the argument's name.
class argument_name {
public:
    /// @brief Specifies the type of argument name match.
    enum class match_type : std::uint8_t {
        m_any, ///< Matches either the primary or the secondary name.
        m_primary, ///< Matches only the primary name.
        m_secondary ///< Matches only the secondary name.
    };
    using enum match_type;

    argument_name() = delete;

    argument_name(const argument_name&) = default;
    argument_name(argument_name&&) = default;

    argument_name& operator=(const argument_name&) = default;
    argument_name& operator=(argument_name&&) = default;

    /**
     * @brief Primary and secondary name constructor.
     * @param primary The primary name of the argument.
     * @param secondary The secondary (short) name of the argument.
     * @param flag_char The flag character (used for optional argument names).
     */
    explicit argument_name(
        std::string primary, std::string secondary = "", char flag_char = flag_char_sentinel
    )
    : _primary(std::move(primary)), _secondary(std::move(secondary)), _flag_char(flag_char) {
        if (this->_primary.empty() and this->_secondary.empty())
            throw std::logic_error("An argument name cannot be empty! At least one of "
                                   "primary/secondary must be specified");
    }

    /// @brief Class destructor.
    ~argument_name() = default;

    /**
     * @brief Equality comparison operator.
     * @param other The argument_name instance to compare with.
     * @return Equality of argument names.
     */
    bool operator==(const argument_name& other) const noexcept {
        return this->_primary == other._primary and this->_secondary == other._secondary;
    }

    /// @brief Checks if the argument name instance has a primary name.
    [[nodiscard]] bool has_primary() const noexcept {
        return not this->_primary.empty();
    }

    /// @brief Checks if the argument name instance has a secondary name.
    [[nodiscard]] bool has_secondary() const noexcept {
        return not this->_secondary.empty();
    }

    /// @return The primary name of the argument.
    [[nodiscard]] const std::string& primary() const noexcept {
        return this->_primary;
    }

    /// @return The secondary (short) name of the argument.
    [[nodiscard]] const std::string& secondary() const noexcept {
        return this->_secondary;
    }

    /// @return The flag character (used for optional argument names).
    [[nodiscard]] char flag_char() const noexcept {
        return this->_flag_char;
    }

    /**
     * @brief Matches the given string to the argument_name instance.
     * @param arg_name The name string to match.
     * @param m_type The match type used to find the argument.
     * @return `true` if the given name matches the primary/secondary name (depending on the match type).
     */
    [[nodiscard]] bool match(std::string_view arg_name, const match_type m_type = m_any)
        const noexcept {
        switch (m_type) {
        case m_any:
            return (this->has_primary() and this->_primary == arg_name)
                or (this->has_secondary() and this->_secondary == arg_name);
        case m_primary:
            return this->has_primary() and this->_primary == arg_name;
        case m_secondary:
            return this->has_secondary() and this->_secondary == arg_name;
        }

        return false;
    }

    /**
     * @brief Matches the given argument name to the argument_name instance.
     * @param arg_name The argument_name instance to match.
     * @return True if arg_name's primary or secondary value matches the argument_name instance.
     */
    [[nodiscard]] bool match(const argument_name& arg_name) const noexcept {
        if (arg_name.has_primary() and this->match(arg_name._primary))
            return true;

        if (arg_name.has_secondary())
            return this->match(arg_name._secondary);

        return false;
    }

    /**
     * @brief Get a string representation of the argument_name.
     */
    [[nodiscard]] std::string str() const noexcept {
        const std::string fc(
            static_cast<std::size_t>(this->_flag_char != flag_char_sentinel), this->_flag_char
        );

        std::string primary_str =
            this->has_primary() ? std::format("{}{}{}", fc, fc, this->_primary) : "";
        std::string separator = this->has_primary() and this->has_secondary() ? ", " : "";
        std::string secondary_str =
            this->has_secondary() ? std::format("{}{}", fc, this->_secondary) : "";

        return std::format("{}{}{}", primary_str, separator, secondary_str);
    }

    /**
     * @brief Stream insertion operator for argument names.
     * @param os The output stream.
     * @param arg_name The argument name to be inserted into the stream.
     * @return The modified output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const argument_name& arg_name) noexcept {
        os << arg_name.str();
        return os;
    }

    // --- constants ---
    static constexpr char flag_char_sentinel = '\0'; ///< Sentinel value for the flag character.

private:
    std::string _primary; ///< The primary name of the argument.
    std::string _secondary; ///< The optional (short) name of the argument.
    char _flag_char; ///< The flag character (used for optional argument names).
};

namespace detail {

/**
 * @brief Argument name member discriminator.
 *
 * This discriminator type is used in the argument initializing methods of the `argument_parser` class.
 */
enum class argument_name_discriminator : bool {
    n_primary, ///< Represents the primary name (used with a long flag prefix --).
    n_secondary ///< Represents the secondary name (used with a short flag prefix --).
};

} // namespace detail

using enum detail::argument_name_discriminator;

} // namespace argon
