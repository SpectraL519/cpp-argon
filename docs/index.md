<style>
  .md-typeset h1 { display: none !important; }
</style>

<div align="center" markdown="1">
  <img src="img/cpp-argon-hex.png" alt="CPP-ARGON" width="360" />
</div>

<br />

<div align="center" markdown="1">

[![GCC](https://github.com/SpectraL519/cpp-argon/actions/workflows/gcc.yaml/badge.svg)](https://github.com/SpectraL519/cpp-argon/actions/workflows/gcc)
[![Clang](https://github.com/SpectraL519/cpp-argon/actions/workflows/clang.yaml/badge.svg)](https://github.com/SpectraL519/cpp-argon/actions/workflows/clang)
[![MSVC](https://github.com/SpectraL519/cpp-argon/actions/workflows/msvc.yaml/badge.svg)](https://github.com/SpectraL519/cpp-argon/actions/workflows/msvc)

</div>

<div align="center" markdown="1">

[![Code Formatting](https://github.com/SpectraL519/cpp-argon/actions/workflows/format.yaml/badge.svg)](https://github.com/SpectraL519/cpp-argon/actions/workflows/format)
[![CPP-ARGON Demo](https://github.com/SpectraL519/cpp-argon/actions/workflows/demo.yaml/badge.svg)](https://github.com/SpectraL519/cpp-argon/actions/workflows/demo)
[![Documentation](https://github.com/SpectraL519/cpp-argon/actions/workflows/documentation.yaml/badge.svg)](https://github.com/SpectraL519/cpp-argon/actions/workflows/documentation)
[![Changelog](https://img.shields.io/badge/changelog-blue.svg?logo=github)](https://github.com/SpectraL519/cpp-argon/releases)

</div>

<br />

## Overview

`CPP-ARGON` is a lightweight, concept-driven command-line argument parsing library for C++20 and newer standards.

Built as a modern alternative to legacy solutions like `boost::program_options`, CPP-ARGON leverages C++20 features to deliver a highly intuitive, fluent API. It abstracts away the boilerplate of building command-line interfaces, offering robust parsing, type-safe value extraction, and automatic help-message generation without the overhead of heavy external dependencies.

> [!IMPORTANT]
>
> Versions of the `CPP-ARGON` library prior to `v4.0.0` were developed under the name `CPP-AP`.
>
> **Legacy Support:** The deprecated URLs (`https://github.com/SpectraL519/cpp-ap` and `.../cpp-ap-demo`) automatically redirect to the new locations, and build targets defined in previous versions remain fully functional.
>
> **Action Required:**
>
> - **Repositories:** We highly recommend updating your remote references and submodules to the new canonical URLs:
>
>   - Library: https://github.com/SpectraL519/cpp-argon
>   - Demo: https://github.com/SpectraL519/cpp-argon-demo

---

## Core Features

- **Fluent & Expressive API:** 

    Chainable configuration methods make defining the parser's attributes, declaring arguments, and setting constraints incredibly simple and intuitive.

- **Highly Generic Argument Types:** 

    Extensively utilizes C++20 concepts to support virtually any custom argument value type, provided it satisfies basic semiregular constraints and is constructible from a `std::string` or overloads the standard extraction operator (`operator>>`).

- **Advanced Argument Grouping:** 

    Organize arguments into logical sections (groups) with strict validation rules, such as `mutually_exclusive()` or `required()`. Apply naming modifiers (prefixes/suffixes) to easily sandbox related flags.

- **Hierarchical Subparsers:** 

    Build complex, nested command-line tools. Each subparser maintains its own independent state, argument groups, and descriptions.

- **Powerful Validation & Actions:** 

    Automatically enforce parameter counts (`nargs`), restrict inputs to specific `choices`, or bind custom C++ callables to observe or modify values on the fly.

- **Flexible Parsing Semantics:** 

    Natively supports POSIX-style short flags, long flags, compound arguments (e.g., `-vvn=5`), inline assignments (e.g., `--number=42`), negative flags (e.g. `--disable-option`), and greedy parsing for variadic inputs.

- **Customizable Unknown Argument Policies:** 

    Choose exactly how the parser reacts to unrecognized flags—fail strictly, warn the user, ignore them entirely, or capture them as values.

---

## Compiler Compatibility

| Compiler | Min Version |
| :---: | :---: |
| **GNU G++** | 13|
| **Clang** | 17|
| **MSVC** | 19.44 (Visual Studio 2022)|

---

## Next Steps

Ready to start building your CLI? Dive into the documentation:

- **[Quick Start](pages/argon/quick_start.md):** Learn how to integrate CPP-ARGON in your project via CMake or Bazel, and define your first parser.
- **[Parser Configuration](pages/argon/parser_cfg.md):** Discover how to customize the parser's behavior, formatting, and unknown argument policies.
- **[Defining Arguments](pages/argon/arguments.md):** Explore positional and optional arguments, boolean flags, and custom value types.
- **[Argument Groups](pages/argon/groups.md):** Group related arguments and enforce validation rules.
- **[Command-Line Parsing](pages/argon/parsing.md):** Understand the parsing rules and how to retrieve extracted values safely.
- **[Subparsers](pages/argon/subparsers.md):** Learn how to build multi-command applications.
- **[Examples](pages/argon/examples.md):** View common use cases and demo projects.
- **[API Reference](pages/api/annotated.md):** Browse the detailed library API documentation.
