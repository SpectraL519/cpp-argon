<p align="center">
  <a href="https://github.com/SpectraL519/cpp-argon">
    <img src="docs/img/cpp-argon-hex.png" alt="CPP-ARGON" width="360" />
  </a>
</p>

<br/>

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

<br/>

## Documentation

The complete, versioned documentation, including tutorials, and the full API reference, is hosted online:

**[Explore the CPP-ARGON Documentation](https://spectral519.github.io/cpp-argon/latest/)**

<br/>
<br/>

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

<br/>
<br/>

## Core Features

- **Fluent & Expressive API:** Chainable configuration methods make defining the parser's attributes, declaring arguments, and setting constraints incredibly simple and intuitive.
- **Highly Generic Argument Types:** Extensively utilizes C++20 concepts to support virtually any custom argument value type, provided it satisfies basic semiregular constraints and is constructible from a `std::string` or overloads the standard extraction operator (`operator>>`).
- **Advanced Argument Grouping:** Organize arguments into logical sections (groups) with strict validation rules, such as `mutually_exclusive()` or `required()`. Apply naming modifiers (prefixes/suffixes) to easily sandbox related flags.
- **Hierarchical Subparsers:** Build complex, nested command-line tools. Each subparser maintains its own independent state, argument groups, and descriptions.
- **Powerful Validation & Actions:** Automatically enforce parameter counts (`nargs`), restrict inputs to specific `choices`, or bind custom C++ callables to observe or modify values on the fly.
- **Flexible Parsing Semantics:** Natively supports POSIX-style short flags, long flags, compound arguments (e.g., `-vvn=5`), inline assignments (e.g., `--number=42`), negative flags (e.g. `--disable-option`), and greedy parsing for variadic inputs.
- **Customizable Unknown Argument Policies:** Choose exactly how the parser reacts to unrecognized flags—fail strictly, warn the user, ignore them entirely, or capture them as values.

<br/>
<br/>

## Compiler Compatibility

| Compiler | Min Version |
| :---: | :---: |
| **GNU G++** | 13 |
| **Clang** | 17 |
| **MSVC** | 19.44 (Visual Studio 2022) |

<br/>
<br/>

## Developer Notes

### Building and Testing

> [!NOTE]
>
> The project uses the [doctest](https://github.com/doctest/doctest) framework for unit testing, however it is already installed in the `tests/external/` directory, so there is no need to install it separately.

#### Build the Testing Executable

```shell
cmake -B build -DBUILD_TESTS=ON
cmake --build build/ # -j<njobs>
```

This will build a `run_uts` executables in the `build/tests/` directory.

#### Running the Tests

To run the project's unit tests, simply execute the testing binary:

```shell
./build/tests/run_uts
```

> [!TIP]
>
> Each test file defines its unique test suite with the same name as the test file. To run tests within a specific test suite, execute:
> ```shell
> ./build/tests/run_uts -ts=<test-suite-name>
> ```

<br/>

### Formatting

> [!NOTE]
>
> The project uses `clang-format-18` which is natively available on the `Ubuntu-24.04 LTS` system.
>
> If the `llvm-18` toolchain isn't available on your system, the installation instructions can be found on the [llvm apt](https://apt.llvm.org/) page. After installing the toolchain run `sudo apt install clang-format-18`.

You can format the code manually using `clang-format-18` or you can use the prepared python script:

```shell
python scripts/format.py
```

To inspect all the script's options and usage, run:

```shell
python scripts/format.py --help
```

> [!NOTE]
>
> The script requires `python >= 3.9`.

<br/>

### Documentation

> [!NOTE]
>
> You can view the online documentation for the latest version [here](https://spectral519.github.io/cpp-argon/latest/).
> To view the documentation for previous versions, use the following URL pattern:
> ```txt
> [https://spectral519.github.io/cpp-argon/](https://spectral519.github.io/cpp-argon/)<version>
> ```

The documentation for this project is built using a modern toolchain combining:

- **Doxygen** (to parse C++ sources into XML), 
- **MkDocs** (with the Material theme and `mkdoxy` plugin to generate the site), 
- **mike** (to handle versioning).

#### Prerequisites

1. **Doxygen**: Ensure Doxygen is installed on your system. For example, on Ubuntu:

```shell
sudo apt update && sudo apt install doxygen -y
```

2. **uv**: Install the `uv` Python package manager:

```shell
pip install uv
```

#### Setup

Before building the documentation for the first time, install and sync the required Python dependencies defined in `pyproject.toml`:

```shell
uv sync
```

#### Building and Serving Locally

The project includes a `Makefile` to completely automate the documentation generation and deployment process.

To build the documentation, deploy it to your local Git history under the `dev` alias, and serve it with live-reloading, run:

```shell
make clean
make docs # uses TAGS="dev" by default
make serve-docs
```

This will start a local server. You can view the documentation by navigating your browser to `http://localhost:8000/dev/`.

#### Makefile Targets

Here are the available Make targets for managing the documentation:

- `make build-docs`: Builds the documentation directly into the `site/` directory without deploying it (useful for quick CI validations).
- `make docs TAGS="<tags>"`: Builds and deploys the documentation to the local `gh-pages` branch using `mike`.
- `make serve-docs`: Starts the local `mike` web server.
- `make clean` (or `make clean-docs`): Removes all generated build directories (`docs/cpp-argon/`, `site/`, and `documentation/`).

<br/>
<br/>

## License

The `CPP-ARGON` project uses the [MIT License](https://mit-license.org/) which can be found in the [LICENSE](LICENSE.md) file.
