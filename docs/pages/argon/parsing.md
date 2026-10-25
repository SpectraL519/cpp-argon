# Parsing & Values

To parse command-line arguments, use of of the parser's available `parse_args` methods:

- Range-based `parse_args(const AR& argv)` method, where `AR` must be a type that satisfies the [`std::ranges::forward_range`](https://en.cppreference.com/w/cpp/ranges/forward_range.html) concept and its value type is convertible to `std::string`.
- The traditional `parse_args(int argc, char* argv[])` method, which works directly with the `argc` and `argv` arguments of the `main` function.

> [!IMPORTANT]
>
> Unless the parser is defined with `argon::dynamic_name` (see [Dynamic Program Name](parser_cfg.md#dynamic-program-name)), the `parse_args(argc, argv)` method ignores the first argument (i.e., `argv[0]`) and is functionally equivalent to calling:
>
> ```cpp
> parse_args(std::span(argv + 1, argc - 1));
> ```

> [!TIP]
>
> The `parse_args` function may throw an `argon::argument_parser_exception` if the configuration of the defined arguments is invalid or the parsed command-line arguments do not match the expected configuration. 
>
> To simplify error handling, the `argument_parser` class provides `try_parse_args` methods which will automatically catch these exceptions, print the error message as well as the help message of the *deepest used parser* (see [Subparsers](subparsers.md)), and exit with a failure status.
>
> Internally, this is equivalent to:
>
> ```cpp
> try {
>     parser.parse_args(...);
> }
> catch (const argon::argument_parser_exception& err) {
>     std::cerr << "[argon::error] " << err.what() << std::endl 
>               << parser.resolved_parser() << std::endl;
>     std::exit(EXIT_FAILURE);
> }
> ```

The simple example below demonstrates how the argument parsing structure should look in your application:

```cpp
#include <argon/argument_parser.hpp>

int main(int argc, char* argv[]) {
    argon::argument_parser parser("some-program"); // (1)!

    parser.version("v0.1.0") // (2)!
          .description("The program does something with command-line arguments")
          .default_arguments(argon::default_argument::o_help);

    parser.add_positional_argument("positional").help("A positional argument"); // (3)!
    parser.add_optional_argument("optional", "o").help("An optional argument");
    parser.add_flag("flag", "f").help("A boolean flag");

    parser.try_parse_args(argc, argv); // (4)!

    std::cout << "positional: " << parser.value("positional") << std::endl // (5)!
              << "optional: " << argon::util::join(parser.values("optional")) << std::endl
              << "flag: " << std::boolalpha << parser.value<bool>("flag") << std::endl;

    return 0;
}
```

1. Create the parser class instance.
2. Define the parser's attributes and default arguments.
3. Define the program arguments.
4. Parse command-line arguments.
5. Use the program's arguments safely.

---

## Basic Argument Parsing Rules

### 1. Optional arguments are parsed only with a flag

An optional argument is recognized only when its primary or secondary flag appears in the command-line input. For example:

```cpp
parser.add_optional_argument("optional", "o");
```

Here, the argument is parsed only if either `--optional` (primary flag) or `-o` (secondary flag) is present. If neither flag is given, the argument is ignored.

> [!IMPORTANT]
> The parser will try to assign the values following such a flag to the specified argument until:
> 
> - **A different argument flag is encountered:**
> 
>   ```cpp
>   parser.add_optional_argument("first", "f");
>   parser.add_optional_argument("second", "s");
>   
>   parser.try_parse_args(argc, argv);
>   
>   std::cout << "first: " << argon::util::join(parser.values("first")) << std::endl
>             << "second: " << argon::util::join(parser.values("second")) << std::endl;
>   ```
>   
>   ```txt
>   > ./program --first value1 value2 --second value3 value4
>   first: value1, value2
>   second: value3, value4
>   ```
> 
> - **The upper bound of the argument's `nargs` parameter is reached:**
>   
>   **NOTE:** By default, an optional argument accepts an arbitrary number of values (no upper bound).
> 
>   ```cpp
>   parser.add_optional_argument<int>("numbers", "n")
>         .nargs(argon::nargs::up_to(3))
>         .help("A list of numbers");
>   ```
> 
>   ```txt
>   > ./program --numbers 1 2 3 4 5
>   [argon::error] Failed to deduce the argument for values [4, 5]
>   ... (help message)
>   ```

### 2. Positional arguments are parsed in the order of definition

Positional arguments are assigned values in the same order they are defined in the program. They are parsed from the command-line input **excluding any values that have already been consumed by optional arguments**. This means positional arguments no longer need to appear at the beginning of the argument list.

```cpp
parser.add_positional_argument("positional1");
parser.add_positional_argument("positional2");

parser.try_parse_args(argc, argv);

std::cout << "positional1: " << parser.value("positional1") << std::endl
          << "positional2: " << parser.value("positional2") << std::endl;
```

**Example execution:**

```txt
> ./program value1 value2
positional1: value1
positional2: value2
```

> [!IMPORTANT]
> * All positional arguments expect **at most one value**.
> * A positional argument's value doesn't have to be present in the command-line only if the argument is defined as **not** required.

### 3. Positional arguments consume free values

A positional argument consumes only those values that cannot be assigned to optional arguments. This allows positional arguments to appear after optional arguments in the command-line input.

```cpp
parser.add_positional_argument("positional1");
parser.add_positional_argument("positional2");
parser.add_optional_argument("optional").nargs(1); // (1)!

parser.try_parse_args(argc, argv);

std::cout << "positional1: " << parser.value("positional1") << std::endl
          << "positional2: " << parser.value("positional2") << std::endl
          << "optional: " << parser.value("optional") << std::endl;
```

1. Limit the number of arguments to 1.

**Example executions:**

```txt
> ./program pos1-value pos2-value --optional opt-value
positional1: pos1-value
positional2: pos2-value
optional: opt-value

> ./program --optional opt-value pos1-value pos2-value
positional1: pos1-value
positional2: pos2-value
optional: opt-value

> ./program pos1-value --optional opt-value pos2-value
positional1: pos1-value
positional2: pos2-value
optional: opt-value
```

> [!TIP]
>
> Because optional arguments accept an arbitrary number of arguments by default, it is a good practice to set the `nargs` parameter for optional arguments where it makes sense.

### 4. Unknown Argument Flag Handling

A command-line argument beginning with a flag prefix (e.g., `--` or `-`) that doesn't match any of the specified optional arguments or a compound of optional arguments is considered **unknown** or **unrecognized**.

By default, an argument parser will throw an exception if an unknown argument flag is encountered. This behavior can be modified using the `unknown_arguments_policy` method.

```cpp
argon::argument_parser parser("unknown-policy-test");

parser.description("A simple test program for unknown argument handling policies")
      .default_arguments(argon::default_argument::o_help)
      .unknown_arguments_policy(argon::unknown_policy::warn); // (1)!

parser.add_optional_argument("known", "k")
      .help("A known optional argument");

parser.try_parse_args(argc, argv);

std::cout << "known = " << argon::util::join(parser.values("known")) << std::endl;
```

1. Set the unknown argument flags handling policy.

The available policies are:

- `argon::unknown_policy::fail` (default) - throws an exception:

  ```txt
  > ./unknown-policy-test --known --unknown
  [argon::error] Unknown argument [--unknown].
  ... (help message)
  ```

- `argon::unknown_policy::warn` - prints a warning message to standard error and continues:
  
  ```txt
  > ./unknown-policy-test --known --unknown
  [argon::warning] Unknown argument '--unknown' will be ignored.
  known =
  ```

- `argon::unknown_policy::ignore` - ignores unknown flags silently:

  ```txt
  > ./unknown-policy-test --known --unknown
  known =
  ```

- `argon::unknown_policy::as_values` - treats unknown argument flags as standard values:

  ```txt
  > ./unknown-policy-test --known --unknown
  known = --unknown
  ```

> [!IMPORTANT]
>
> - The unknown argument policy only affects the parser's behaviour when calling `parse_args` or `try_parse_args`.
> - When parsing known args with `parse_known_args` or `try_parse_known_args` (see [Parsing Known Arguments](#parsing-known-arguments)), all unknown arguments (flags and values) are collected and returned as the parsing result, entirely ignoring the specified policy.

### 5. Inline Value Assignment

By default, optional arguments accept values separated by spaces (e.g., `--number 42`). CPP-ARGON also natively supports POSIX-style inline value assignment using the `=` character.

```cpp
parser.add_argument<argon::none_type>("verbose", "v");
parser.add_optional_argument<int>("number", "n");
parser.add_optional_argument("string", "s");
```

You can use the assignment character with primary flags, secondary flags, and even at the end of [compound flags](#6-parsing-compound-arguments):

```txt
> ./program --number=42 -s=hello
> ./program -vvn=5
```

> [!WARNING]
>
> - **Do not place spaces around the assignment character.** Command-line shells split arguments by spaces before the program ever processes them. Typing `--number = 42` causes the parser to treat `=` as the value for `--number`.
> - If your assigned value contains spaces, quote the value directly after the assignment character:
> ```txt
> > ./program --string="hello world"
> ```

**Multiple Values**

If an argument is configured to accept multiple values, e.g.:

```cpp
parser.add_optional_argument("names", "n")
      .nargs(argon::nargs::at_least(1));
```

you can seamlessly combine inline assignment for the first value with standard space-separated values for the rest:

```txt
> ./program --names=Kowalski Wisniewski Nowak
```

### 6. Parsing Compound Arguments

Compound argument flags are **secondary** argument flags where **every** character matches the secondary name of an optional argument.

```cpp
parser.add_optional_argument("verbose", "v")
      .nargs(0)
      .help("Increase verbosity level");

parser.add_flag("option", "o")
      .help("Enable an option flag");

parser.add_optional_argument<int>("numbers", "n")
      .help("Provide integer values");

parser.try_parse_args(argc, argv);

std::cout << "Verbosity level: " << parser.count("verbose")
          << "\nOption used: " << std::boolalpha << parser.value<bool>("option")
          << "\nNumbers: " << argon::util::join(parser.values<int>("numbers"), ", ")
          << std::endl;
```

**Example execution:**

```txt
> ./program -vvon 1 2 3
Verbosity level: 2
Option used: true
Numbers: 1, 2, 3
```

> [!IMPORTANT]
>
> - If there exists an argument whose secondary name perfectly matches a possible compound of other arguments, the parser treats the flag as the **single matching argument**, not as multiple flags.
> - The parser will attempt to assign any trailing values to the argument represented by the **last character** of the compound flag.

#### Compound Flags within Argument Groups

If you organized arguments into an argument group using a prefix and/or suffix, compound flags must strictly start with the group's prefix and end with the group's suffix. Characters nested between them will be treated as secondary names of the arguments registered exclusively to that group.

```cpp
auto& log_opts = parser.add_group("Logging Options")
                       .with_prefix("log-")
                       .with_suffix("-opt");

log_opts.add_optional_argument<argon::none_type>("verbose", "v");
log_opts.add_optional_argument<argon::none_type>("quiet", "q");
log_opts.add_optional_argument<argon::none_type>("debug", "d");

parser.try_parse_args(argc, argv);
```

```txt
> ./program -log-vvqd-opt
```

*This is equivalent to passing: `-log-v-opt -log-v-opt -log-q-opt -log-d-opt*`

> [!NOTE]
>
> The parser automatically handles prefix overlaps. If you type `-log` and your group prefix is `log` but no valid group compound can be formed, the parser safely falls back to standard parsing (evaluating it as a multi-character secondary flag if such exists, or a compound flag `-l`, `-o`, `-g` if those individual arguments exist).

---

## Parsing Known Arguments

If you wish to handle only specific command-line arguments and ignore any unrecognized ones, use the `parse_known_args` method.

This method behaves similarly to `parse_args()`, but it does not throw an error if unknown arguments are detected. Instead, it returns all the unknown arguments (both flags and values) as a `std::vector<std::string>`.

```cpp
parser.add_optional_argument("recognized", "r")
      .nargs(argon::nargs::up_to(2))
      .help("A recognized optional argument");

const auto unknown_args = parser.parse_known_args(argc, argv); // (1)!

std::cout << "recognized = " << argon::util::join(parser.values("recognized")) << std::endl
          << "unknown = " << argon::util::join(unknown_args) << std::endl;
```

1. Unrecognized arguments are safely stored in `unknown_args`.

**Example execution:**

```txt
> ./program value0 --recognized value1 value2 value3 --unrecognized value
recognized = value1, value2
unknown = value0, value3, --unrecognized, value
```

> [!IMPORTANT]
>
> If a parser encounters an unrecognized argument flag during *known* args parsing, the currently processed optional argument is immediately reset. This means any values following the unrecognized flag will be used to parse positional arguments or treated as unknown arguments.
>
> ```txt
> > ./program --recognized value1 value2 --unrecognized value4 --recognized value5
> recognized = value1, value2, value5
> unknown = --unrecognized, value4
> ```

> [!TIP]
>
> Similarly to `parse_args`, `parse_known_args` has a `try` equivalent (`try_parse_known_args`) which will automatically catch configuration exceptions, print the error message, and exit with a failure status.

---

## Retrieving Argument Values

Once parsing is complete, you can extract your data using three different interfaces: via the parser, via direct argument references, or via argument groups.

### Using the Parser

You can retrieve an argument's value(s) using the parser instance by providing the argument's registered name:

```cpp
/*const*/ value_type value /*&*/ = parser.value<value_type>("argument_name"); // (1)!
/*const*/ value_type value = parser.value_or<value_type>("argument_name", fallback_value); // (2)!
const std::vector<value_type>& values = parser.values<value_type>("argument_name"); // (3)!
```

1. Returns the argument's parsed or predefined value. If multiple values were parsed, returns the first one. For simple types (e.g. booleans, integers) this returns by value. Otherwise, it returns by reference.
2. Returns the argument's value, or the specified fallback value if the argument has no values. Always returns by value.
3. Returns a vector of the argument's values. Returns an empty vector if no values exist.

> [!NOTE]
>
> The argument value getter functions might throw an exception if:
> - An argument with the given name does not exist.
> - The argument does not contain any values (only applies to getter `1!`).
> - The specified `value_type` does not match the true value type of the argument.

### Using Argument References

When you define an argument, the parser returns a strongly-typed reference to the argument object. You can use this reference to retrieve the argument's state and values directly.

Because the reference is inherently aware of its own `value_type`, you **do not** need to provide template parameters when calling these methods, making it entirely type-safe.

```cpp
auto& port_arg = parser.add_optional_argument<int>("port", "p"); // (1)!
auto& verbose_arg = parser.add_optional_argument<argon::none_type>("verbose", "v");

parser.try_parse_args(argc, argv);

int port = port_arg.value_or(8080); // (2)!
std::size_t verbosity_level = verbose_arg.count(); // (3)!

if (port_arg.is_used() and verbosity_level >= 2) {
    std::cout << "Port explicitly set to " << port << '\n';
}
```

1. Capture the returned references during configuration.
2. Retrieve values directly without template parameters.
3. Query argument state (`count` tracks the number of times the flag was used).

The argument instance provides the exact same API as the parser (`value()`, `value_or()`, `values()`, `has_value()`, `is_used()`, `count()`), but tied directly to its own state.

### Using Argument Groups

If you organize your arguments into an `argument_group` configured with a *prefix* and/or *suffix*, retrieving values via the `argument_parser` requires you to use the fully formatted name (e.g., `parser.value("out-file-cfg")`).

To simplify this, the `argument_group` class provides its own value and state getters. These methods automatically apply the group's naming modifiers, allowing you to query arguments using their simple base names.

```cpp
auto& out_opts = parser.add_group("Output").with_prefix("out-");

out_opts.add_optional_argument("file"); // (1)!
out_opts.add_optional_argument<argon::none_type>("verbose", "v");

parser.try_parse_args(argc, argv);

std::size_t verbosity_level = parser.count("out-verbose"); // (2)!

std::size_t verbosity_level_grp = out_opts.count("verbose"); // (3)!
std::string out_file = out_opts.value_or("file", "default.txt");

if (not out_opts.is_used("file")) {
    std::cout << "No file specified - falling back to a default file\n";
}
std::cout << "Writing to " << out_file << " at verbosity level " << verbosity_level_grp << std::endl;
```

1. Registered in the underlying parser as `"out-file"`.
2. Querying the parser requires the full modified name.
3. Querying the group requires ONLY the base name.
