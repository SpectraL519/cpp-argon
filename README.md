<p align="center">
  <a href="https://github.com/SpectraL519/cpp-argon">
    <img src="docs/img/cpp-argon-hex.png" alt="CPP-ARGON" width="360" />
  </a>
</p>

<br />

<div align="center">

[![GCC](https://github.com/SpectraL519/cpp-argon/actions/workflows/gcc.yaml/badge.svg)](https://github.com/SpectraL519/cpp-argon/actions/workflows/gcc)
[![Clang](https://github.com/SpectraL519/cpp-argon/actions/workflows/clang.yaml/badge.svg)](https://github.com/SpectraL519/cpp-argon/actions/workflows/clang)
[![MSVC](https://github.com/SpectraL519/cpp-argon/actions/workflows/msvc.yaml/badge.svg)](https://github.com/SpectraL519/cpp-argon/actions/workflows/msvc)

</div>

<div align="center">

[![Code Formatting](https://github.com/SpectraL519/cpp-argon/actions/workflows/format.yaml/badge.svg)](https://github.com/SpectraL519/cpp-argon/actions/workflows/format)
[![CPP-ARGON Demo](https://github.com/SpectraL519/cpp-argon/actions/workflows/demo.yaml/badge.svg)](https://github.com/SpectraL519/cpp-argon/actions/workflows/demo)
[![Documentation](https://github.com/SpectraL519/cpp-argon/actions/workflows/documentation.yaml/badge.svg)](https://github.com/SpectraL519/cpp-argon/actions/workflows/documentation)
[![Changelog](https://img.shields.io/badge/changelog-blue.svg?logo=github)](https://github.com/SpectraL519/cpp-argon/releases)

</div>

<br />

## Documentation

The complete, versioned documentation, including tutorials, and the full API reference, is hosted online:

**[Explore the CPP-ARGON Documentation](https://spectral519.github.io/cpp-argon/latest/)**

<br />

## Overview

`CPP-ARGON` is a lightweight, concept-driven command-line argument parsing library for C++20 and newer standards.

Built as a modern alternative to legacy solutions like `boost::program_options`, CPP-ARGON leverages C++20 features to deliver a highly intuitive, fluent API. It abstracts away the boilerplate of building command-line interfaces, offering robust parsing, type-safe value extraction, and automatic help-message generation without the overhead of heavy external dependencies.

> [!CAUTION]
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

> [!NOTE]
>
> [v1.0](https://github.com/SpectraL519/cpp-argon/releases/tag/v1.0) of the library has been developed for the *Team Programming* course at the *Wrocław University of Science and Technology*.
>
> Faculty: *W04N - Faculty of Information and Communication Technology*
>
> Field of study: *Algorithmic Computer Science*
>
> The project received the 1st place at the 2024 CreatiWITy competition organized by the faculty. The article in Polish can be found on the [faculty website](https://wit.pwr.edu.pl/aktualnosci/oto-laureaci-konkursu-creatiwity-273.html). Please note that this is not a technical article :)

<br />

## Core Features

- **Fluent & Expressive API:** Chainable configuration methods make defining the parser's attributes, declaring arguments, and setting constraints incredibly simple and intuitive.
- **Highly Generic Argument Types:** Extensively utilizes C++20 concepts to support virtually any custom argument value type, provided it satisfies basic semiregular constraints and is constructible from a `std::string` or overloads the standard extraction operator (`operator>>`).
- **Advanced Argument Grouping:** Organize arguments into logical sections (groups) with strict validation rules, such as `mutually_exclusive()` or `required()`. Apply naming modifiers (prefixes/suffixes) to easily sandbox related flags.
- **Hierarchical Subparsers:** Build complex, nested command-line tools. Each subparser maintains its own independent state, argument groups, and descriptions.
- **Powerful Validation & Actions:** Automatically enforce parameter counts (`nargs`), restrict inputs to specific `choices`, or bind custom C++ callables to observe or modify values on the fly.
- **Flexible Parsing Semantics:** Natively supports POSIX-style short flags, long flags, compound arguments (e.g., `-vvn=5`), inline assignments (e.g., `--number=42`), negative flags (e.g. `--disable-option`), and greedy parsing for variadic inputs.
- **Customizable Unknown Argument Policies:** Choose exactly how the parser reacts to unrecognized flags—fail strictly, warn the user, ignore them entirely, or capture them as values.

<br />

## Compiler Compatibility

| Compiler | Min Version |
| :---: | :---: |
| **GNU G++** | 13 |
| **Clang** | 17 |
| **MSVC** | 19.44 (Visual Studio 2022) |

<br />

## Developer Notes

To build the testing and documentation suites locally, follow the guides below.

### Building and Testing

The project uses the [doctest](https://github.com/doctest/doctest) framework for unit testing. It is already installed in the `tests/external/` directory, so there is no need to install it separately.

```bash
# Configure the project with tests enabled
cmake -B build -DBUILD_TESTS=ON

# Build the test executable
cmake --build build/ -j<njobs>

# Execute the tests
./build/tests/run_uts

```

> [!TIP]
> Each test file defines its unique test suite with the same name as the test file. To run tests within a specific test suite, execute: `./build/tests/run_uts -ts=<test-suite-name>`

### Formatting

The project uses `clang-format-18` which is natively available on the `Ubuntu-24.04 LTS` system. You can format the code manually using `clang-format-18` or you can use the prepared python script:

```bash
python scripts/format.py

```

### Building the Documentation

The documentation is built using a modern toolchain combining **Doxygen** (to parse C++ sources into XML), **MkDocs** (with the Material theme and `mkdoxy` plugin to generate the site), and **mike** (to handle versioning).

Ensure **Doxygen** and **uv** are installed on your system. Before building for the first time, install and sync the required Python dependencies:

```bash
uv sync
```

To build the documentation, deploy it to your local Git history under the `latest` alias, and serve it with live-reloading, run:

```bash
make clean
make docs TAGS="latest"
make serve-docs
```

## License

The `CPP-ARGON` project uses the [MIT License](https://mit-license.org/) which can be found in the [LICENSE](LICENSE.md) file.
