# Parser Configuration

To use the argument parser in your code, you need to instantiate the `argon::argument_parser` class. This class acts as the central hub for defining your command-line interface, configuring its behavior, and parsing the input.

```cpp
argon::argument_parser parser("myprog"); // (1)!
parser.version("v0.1.0") // (2)!
      .description("Description of the program")
      .formatting({.indent_width = 4u, .max_line_width = 80ull})
      .verbose();
```

1. Instantiate the parser by providing the program name.
2. The parser can be fully configured using a fluent API.

The following sections detail all available configuration attributes.

---

## Program Name

When creating an argument parser instance, you must provide a program name to the constructor. This name is used in the generated help message.

> [!IMPORTANT]
>
> If you provide a strict string (e.g., `"myprog"`), it cannot be empty and **must not contain whitespace characters**.

### Dynamic Program Name

If you prefer not to hardcode the program name, you can instruct the parser to dynamically deduce it from the command-line input (specifically `argv[0]`) by initializing it with the `argon::dynamic_name` tag:

```cpp
argon::argument_parser parser(argon::dynamic_name); // (1)!
parser.try_parse_args(argc, argv); // (2)!
```

1. Initialize with the dynamic name tag.
2. The program name is automatically resolved from `argv[0]` when parsing begins.

When the parser processes `argc` and `argv`, it extracts the executable's base name and cleanly strips any preceding directory paths (e.g., `/usr/bin/my_app` or `C:\path\to\my_app.exe` becomes `my_app`). This resolved name is automatically propagated down to all of its [subparsers](subparsers.md).

> [!WARNING]
>
> If you configure a parser with `argon::dynamic_name`, you **must** use the `argc, argv` parsing overloads (like `parse_args(argc, argv)`). Attempting to use the range-based parsing functions (e.g., passing a `std::span` or `std::vector`) before the parser has resolved its name will result in a `std::logic_error`.

---

## Version and Description

You can provide a version and a description for your program. These are displayed at the very top of the parser's configuration output (e.g., when the user passes the `--help` flag).

```cpp
parser.version("v1.0.0")
      .description("A high-performance file processing utility.");

```

> [!WARNING]
>
> The version string cannot contain control characters (such as line breaks or tabs). Attempting to use them will throw an `argon::invalid_configuration` exception.

---

## Help Message Formatting

You can customize the layout of the parser's automatically generated help message by passing an `argon::format_config` object to the `formatting()` method. This allows you to control the indentation of argument descriptions and enable automatic text wrapping for long help messages.

```cpp
parser.formatting({
    .indent_width = 4u,      // (1)!
    .max_line_width = 80ull  // (2)!
});

```

1. Set section indentation to 4 spaces (default: 2).
2. Wrap help text at 80 characters (default: 120).

> [!NOTE]
>
> - Setting `max_line_width` to `0` disables automatic text wrapping entirely. 
> - Manual line breaks (`\n`) in your help strings are always respected and properly aligned regardless of the `max_line_width` setting.

---

## Flag Character

By default, the parser uses the POSIX-standard hyphen (`-`) as the flag prefix character (e.g., `-h`, `--help`). You can change this to support different CLI styles, such as Windows-style slashes (`/`), by setting a custom flag character.

```cpp
argon::argument_parser parser("program");

parser.flag_char('/'); // (1)!

parser.add_optional_argument("help", argon::n_secondary) // (2)!
      .help("Display the help message");

parser.try_parse_args(argc, argv);

```

1. Change the flag character to a forward slash.
2. Force the usage of a single flag character by making the `help` name secondary.

**Example execution:**

```txt
> ./program /help
```

> [!WARNING]
>
> - The flag character must be a printable ASCII character and cannot be the same as the assignment character (`=`).
> - You **must** set the flag character before adding any arguments to the parser. Attempting to change it after arguments have been added will result in an `argon::invalid_configuration` exception.

---

## Verbosity

The verbosity mode is `false` by default. If enabled using `.verbose()`, the parser's help output will include highly detailed information about each argument's configuration parameters (such as its `nargs` bounds, valid choices, default values, and implicit values) appended to the standard name and help message.

```cpp
parser.verbose(true);
```

---

## Unknown Arguments Policy

By default, the parser throws an exception if it encounters a command-line argument that it does not recognize. You can alter this behavior (e.g., to ignore them, warn the user, or treat them as positional values) using the `unknown_arguments_policy()` method.

For detailed information on the available policies and their behaviors, see [Unknown Argument Flag Handling](parsing.md#unknown-argument-flag-handling).

---

## Resetting Parser State

If you need to reuse an `argument_parser` instance multiple times within the same program execution (e.g., inside a REPL loop, a long-running service, or a test suite), you can clear its parsing state using the `reset()` method:

```cpp
parser.reset();
```

Calling `reset()` performs a deep clean of the runtime state while keeping your configuration entirely intact. Specifically, it will:

- Clear all explicitly parsed values and reset argument usage counts to zero.
- Lift any masks on default or implicit values that were suppressed by negative flags (e.g., flags utilizing `argon::action::discard_all`).
- Revert the parser's `invoked()` and `finalized()` state flags to `false`.
- Recursively reset the state of all attached [subparsers](subparsers.md).
- Revert dynamically deduced program names (`argon::dynamic_name`) back to an unresolved state, ensuring the parser safely re-deduces its name from `argv[0]` on the next run.

> [!IMPORTANT]
>
> `reset()` only clears the **runtime parsing state**. It does not alter, remove, or destroy your parser's structural configuration (defined arguments, groups, explicit default values, choices, or actions).

> [!NOTE]
>
> The parser does **not** call `reset()` automatically when you invoke `parse_args()`. This deliberate design choice preserves the parser's state across multiple calls, enabling advanced multi-pass parsing workflows (for example, parsing a string of base configurations first, and then parsing user CLI overrides to accumulate values). If your intent is to start a completely fresh parsing run, you must call `reset()` explicitly.
