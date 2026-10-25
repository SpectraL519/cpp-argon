# Defining Arguments

The `CPP-ARGON` parser supports two fundamental types of arguments: **positional** and **optional**. 

> [!NOTE]
>
> General rules for parsing arguments from the command line are described on the [Command-Line Parsing](parsing.md) page.

---

## Adding Arguments

To add an argument to your parser, use the strongly-typed `add_positional_argument` and `add_optional_argument` template methods.

### Syntax and Names

```cpp
parser.add_positional_argument<std::string>("input"); // (1)!

parser.add_optional_argument<int>("count", "c"); // (2)!

parser.add_optional_argument("v", argon::n_secondary); // (3)!
```

1. Positional arguments must have exactly one name (no secondary/short names allowed).
2. Optional arguments can have both a primary (long) name and a secondary (short) name.
3. Optional arguments can also be defined using *only* a secondary name. The default argument's value type is `std::string`.

### Value Types

An argument's value type defaults to `std::string` if no template parameter is provided.

> [!IMPORTANT]
>
> An argument's value type must be `argon::none_type` **or** satisfy all of the following requirements:
> - The type is [constructible from](https://en.cppreference.com/w/cpp/concepts/constructible_from) `const std::string&` or overloads `std::istream& operator>>`. The parser will always try direct initialization from `std::string` first, and only fall back to the extraction operator if direct initialization fails.
> - The type satisfies the [`std::semiregular`](https://en.cppreference.com/w/cpp/concepts/semiregular.html) concept (default-initializable and copyable).

> [!NOTE]
>
> If the argument's value type is `argon::none_type`, the argument will not accept any values and therefore no value-related parameters can be set for such an argument. This includes the [number of arguments](#number-of-arguments--nargs), [greedy parsing](#greedy-parsing--greedy), [value choices](#value-choices--choices), [default values](#default-arguments), [implicit values](#implicit-values--implicit_values), and [value actions](#value-actions--actions).

### Boolean Flags

Flags are essentially optional arguments with a boolean value type. The `add_flag` method provides a convenient shorthand.

```cpp
parser.add_flag("enable_option", "e").help("Enables an option"); // (1)!

parser.add_flag<false>("disable_option", "d").help("Disables an option"); // (2)!
```

1. By default, flags store `true` when parsed from the command-line.
2. Passing `<false>` inverts this behavior, storing `true` by default, and `false` when the flag is encountered.

---

## Argument Parameters

Once an argument is added, you can configure it using a fluent API (method chaining).

### Help Message | `.help`

Sets the argument's description which will be printed in the auto-generated help message.

```cpp
parser.add_optional_argument("legacy-feature")
      .help("Enables the legacy processing mode.\n\nWARNING: This feature is deprecated."); // (1)!
```

1. The help string fully supports manual line breaks (`\n`). The parser's formatting engine automatically aligns all subsequent lines to the correct indentation offset.

### Argument Hiding | `.hidden`

If this option is set, the argument will be excluded from the program's help description.

```cpp
parser.add_optional_argument("secret-dev-mode").hidden();
```

### Required Arguments | `.required`

Enforces that an argument must be provided by the user. If a required argument is missing, parsing will throw an exception.

```cpp
parser.add_positional_argument("input"); // (1)!
parser.add_positional_argument("output").required(false); // (2)!

parser.add_optional_argument("config", "c").required(); // (3)!
```

1. Positional arguments are required by default.
2. Making a positional argument optional. Note: If a positional argument is non-required, no required positional arguments can be defined after it.
3. Optional arguments are `required(false)` by default.

### Argument Checks Suppression | `.suppress_arg_checks`

If a suppressing argument is explicitly used in the command-line, requirement validation (such as `required` or `nargs` bounds) is skipped for all other arguments. This is typically used for flags like `--help` or `--version`.

```cpp
parser.add_optional_argument("version", "v").suppress_arg_checks();
```

> [!WARNING]
>
> - Enabling `suppress_arg_checks` has no effect on [argument group](groups.md) requirements validation.
> - A required argument cannot suppress checks (an exception will be thrown if both attributes are used for one argument).

### Number of Arguments | `.nargs`

Sets the allowed number of values to be parsed for an argument.

```cpp
parser.add_optional_argument("input", "i").nargs(1); // (1)!
parser.add_optional_argument("range").nargs(1, 3); // (2)!
parser.add_optional_argument("more").nargs(argon::nargs::more_than(1)); // (3)!
parser.add_optional_argument("any").nargs(argon::nargs::any()); // (4)!
```

1. Exact number of required values.
2. Fully bound range (between 1 and 3 values).
3. Partially bound ranges (`at_least`, `more_than`, `less_than`, `up_to`).
4. Unbound range (accepts arbitrary values).

> [!IMPORTANT]
>
> The default `nargs` is `argon::nargs::range(1ull)` for positional arguments, and `argon::nargs::any()` for optional arguments.

### Greedy Parsing | `.greedy`

If enabled, the argument consumes ALL trailing command-line values until its upper `nargs` bound is reached, bypassing standard flag detection.

```cpp
parser.add_optional_argument("args")
      .greedy()
      .help("Consumes everything, even things that look like --flags");
```

### Value Choices | `.choices`

Restricts valid inputs to a specific set of values. Attempting to parse a value outside this list results in an error.

```cpp
parser.add_optional_argument<char>("method", "m").choices('a', 'b', 'c'); // (1)!
```

1. Can be called with variadic values or an arbitrary `std::ranges::range`. The argument's `value_type` must be equality comparable (`==`).

### Value Actions | `.actions`

Actions are standard C++ callables executed after parsing an argument's value. The library automatically deduces whether the action is an **observer** or a **modifier**.

```cpp
void validate_usertag(const std::string& tag) { // (1)!
    if (tag.empty() or tag.front() != '@') 
        throw std::runtime_error("Invalid tag");
}

void capitalize(std::string& s) { // (2)!
    if (not s.empty()) 
      s[0] = std::toupper(s[0]);
}

parser.add_optional_argument("user", "u")
      .actions(validate_usertag, capitalize); // (3)!
```

1. **Observe Action** (`void(const value_type&)`): Validates or logs the value without modifying it.
2. **Modify Action** (`void(value_type&)`): Mutates the parsed value in-place.
3. You can chain multiple actions together in a single call.

### Default Values | `.default_values`

A list of fallback values to use if no values are provided in the command line.

```cpp
parser.add_optional_argument("output", "o").default_values("output.txt");
```

> [!WARNING]
>
> Setting `default_values` automatically disables the `required` option for that argument.

---

## Optional Argument Parameters

The following configuration parameters are exclusive to optional arguments.

### Implicit Values | `.implicit_values`

Values that are injected if the argument's flag is provided without any accompanying values.

```cpp
parser.add_optional_argument("save", "s").implicit_values("output.txt"); // (1)!
```

1. If the user passes `--save` with no value, it resolves to `"output.txt"`. If the user omits the flag entirely, the argument remains empty.

### On-flag Actions | `.actions`

Functions called immediately upon encountering the argument's flag, before parsing any values.

```cpp
parser.add_optional_argument("debug")
      .actions<argon::action::on_flag>(
        []() { std::cout << "Debug mode enabled\n"; }
      );
```

### Negative Flags | `.negative`

Registers an inverted prefix (e.g., `"no-"`) allowing users to explicitly disable features or clear state.

```cpp
parser.add_optional_argument("color", "c")
      .default_values("blue")
      .negative("no-"); // (1)!

parser.add_optional_argument<int>("numbers")
      .negative("clear-", argon::action::discard_parsed); // (2)!
```

1. Registers a `--no-color` flag. By default, using it discards all values (`argon::action::discard_all`) or inverts a boolean flag.
2. Customizes the negative behavior to only clear user-parsed values, preserving defaults.

---

## Predefined Parameter Values

### Actions

- `print_help` | on-flag
  
  Prints the parser's help message to the output stream and optionally exits with the given code.
  
  ```cpp
  argon::action::util::state_callable_type print_help(
      const argon::argument_parser& parser,
      const std::optional<int> exit_code = std::nullopt,
      std::ostream& os = std::cout
  ) noexcept;
  ```

- `check_file_exists` | observe (value type: `std::string`)

  Throws if the provided file path does not exist.

  ```cpp
  auto check_file_exists() noexcept;
  ```

- `gt` | observe (value type: arithmetic)

  Validates that the value is strictly greater than `lower_bound`.

  ```cpp
  template <argon::traits::c_arithmetic T>
  auto gt(const T lower_bound) noexcept;
  ```

- `geq` | observe (value type: arithmetic)

  Validates that the value is greater than or equal to `lower_bound`.

  ```cpp
  template <argon::traits::c_arithmetic T>
  auto geq(const T lower_bound) noexcept;
  ```

- `lt` | observe (value type: arithmetic)

  Validates that the value is strictly less than `upper_bound`.

  ```cpp
  template <argon::traits::c_arithmetic T>
  auto lt(const T upper_bound) noexcept;
  ```

- `leq` | observe (value type: arithmetic)

  Validates that the value is less than or equal to `upper_bound`.

  ```cpp
  template <argon::traits::c_arithmetic T>
  auto leq(const T upper_bound) noexcept;
  ```

- `within` | observe (value type: arithmetic)

  Checks if the value is within the given interval. Bound inclusivity is customizable using template parameters.

  ```cpp
  template <traits::c_arithmetic T, bool LeftInclusive = true, bool RightInclusive = true>
  auto within(const T lower_bound, const T upper_bound) noexcept;
  ```

---

## Default Arguments

The `CPP-ARGON` library defines several default arguments, which can be added to the parser's configuration as follows.

```cpp
parser.default_arguments(<args>);
```

> [!NOTE]
>
> The `default_arguments` function can be called with:
> - A variadic number of `argon::default_argument` values
> - An arbitrary [`std::ranges::range`](https://en.cppreference.com/w/cpp/ranges/range.html) type with the `argon::default_argument` value type

The library defines the following default arguments:

- `p_input`: Positional input file path argument.

  ```cpp
  parser.add_positional_argument<std::string>("input") // (1)!
        .actions(argon::action::check_file_exists())
        .help("Input file path");
  ```

1. Equivalent parser configuration.

- `p_output`: Positional output file path argument.

  ```cpp
  parser.add_positional_argument("output").help("Output file path"); // (1)!
  ```

  1. Equivalent parser configuration.

- `o_help`: No-value flag that prints the parser's help message.

  ```cpp
  parser.add_optional_argument<argon::none_type>("help", "h") // (1)!
        .actions<argon::action::on_flag>(argon::action::print_help(parser, EXIT_SUCCESS))
        .help("Display the help message");
  ```

  1. Equivalent parser configuration.

- `o_version`: No-value flag that prints the parser's version info.
  
  ```cpp
  parser.add_optional_argument<argon::none_type>("version", "v") // (1)!
        .actions<argon::action::on_flag>([&parser]() {
            parser.print_version();
            std::exit(EXIT_SUCCESS);
        })
        .help("Display program's version info");
  ```

  1. Equivalent parser configuration.

- `o_input` and `o_multi_input`: Optional input file path(s) arguments.

  ```cpp
  parser.add_optional_argument("input", "i") // (1)!
        .required()
        .nargs(1)
        .actions(argon::action::check_file_exists())
        .help("Input file path");

  parser.add_optional_argument("input", "i") // (2)!
        .required()
        .nargs(argon::nargs::at_least(1))
        .actions(argon::action::check_file_exists())
        .help("Input files paths");
  ```

  1. Equivalent configuration for `o_input`.
  2. Equivalent configuration for `o_multi_input`.

- `o_output` and `o_multi_output`: Optional output file path(s) arguments.

  ```cpp
  parser.add_optional_argument("output", "o") // (1)!
        .required()
        .nargs(1)
        .help("Output file path");

  parser.add_optional_argument("output", "o") // (2)!
        .required()
        .nargs(argon::nargs::at_least(1))
        .help("Output files paths");
  ```

  1. Equivalent configuration for `o_output`.
  2. Equivalent configuration for `o_multi_output`.
