# Quick Start

This guide will walk you through integrating `CPP-ARGON` into your C++ project and defining your very first command-line argument parser.

---

## Installation & Integration

`CPP-ARGON` is designed to be easily dropped into modern C++ development environments.

### CMake Integration

For CMake projects, you can fetch the library directly in your `CMakeLists.txt` file using the `FetchContent` module:

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.14)

project(my_project LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    cpp-argon
    GIT_REPOSITORY https://github.com/SpectraL519/cpp-argon.git
    GIT_TAG v5.0.0 # (1)!
)

FetchContent_MakeAvailable(cpp-argon)

add_executable(my_project main.cpp) # (2)!

set_target_properties(my_project PROPERTIES
    CXX_STANDARD 20 # (3)!
    CXX_STANDARD_REQUIRED YES
)

target_link_libraries(my_project PRIVATE cpp-argon) # (4)!
```

1. Declare the desired CPP-ARGON version
2. Define your executable
3. CPP-ARGON requires C++20 or newer
4. Link against the cpp-argon library

### Bazel Build System

To use `CPP-ARGON` in a [Bazel](https://bazel.build/) project, add the following to your `MODULE.bazel` (or `WORKSPACE.bazel`) file:

```bazel
# MODULE.bazel
git_repository = use_repo_rule("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "cpp-argon",
    remote = "https://github.com/SpectraL519/cpp-argon.git",
    tag = "v5.0.0" # (1)!
)
```

1. Declare the desired CPP-ARGON version

> [!IMPORTANT]
>
> CPP-ARGON versions older than [v2.5.0](https://github.com/SpectraL519/cpp-argon/releases/tag/v2.5.0) do not support building with Bazel.

Then, add the `"@cpp-argon//:cpp-argon"` dependency to the target where you want to use it by adding it to the `deps` list:

```bazel
# BUILD.bazel
cc_binary(
    name = "my_app",
    srcs = ["main.cpp"],
    deps = ["@cpp-argon//:cpp-argon"],
    cxxopts = ["-std=c++20"],
    visibility = ["//visibility:public"],
)
```

### Downloading the Library (Manual)

If you are not using a package manager or a build system capable of fetching dependencies, you can download the desired [library release](https://github.com/SpectraL519/cpp-argon/releases) archive. Extract it into your desired directory and simply add `<cpp-argon-dir>/include` to the include directories of your compiler.

---

## Your First Parser

Once the library is integrated, you can start building your CLI. Here is a complete, minimal example demonstrating how to initialize the parser, define some basic arguments, and retrieve their parsed values.

```cpp
#include <argon/argument_parser.hpp>

int main(int argc, char* argv[]) {
    argon::argument_parser parser("my-program"); // (1)!

    parser.version("v1.0.0") // (2)!
          .description("A simple application demonstrating CPP-ARGON.")
          .default_arguments(argon::default_argument::o_help);

    parser.add_positional_argument("input") // (3)!
          .help("The primary input file or directory");

    parser.add_optional_argument<int>("count", "c")
          .default_values(1)
          .help("Number of times to process the input");

    parser.add_flag("verbose", "v")
          .help("Enable verbose logging output");

    parser.try_parse_args(argc, argv); // (4)!

    std::cout << "Input path  : " << parser.value("input") << '\n' // (5)!
              << "Count       : " << parser.value<int>("count") << '\n'
              << "Verbose mode: " << std::boolalpha << parser.value<bool>("verbose") << '\n';

    return 0;
}
```

1. Create the parser instance
2. Configure the parser's metadata and inject default arguments (like `--help`)
3. Define your arguments
4. Parse the command-line inputs. `try_parse_args` automatically handles exceptions and prints error/help messages
5. Retrieve and use the parsed values

### Testing the Output

Compile and run the program to see the parser in action:

**Printing the help message:**

```text
> ./my-program --help
Program: my-program v1.0.0

  A simple application demonstrating CPP-ARGON.

Positional Arguments:

  input : The primary input file or directory

Optional Arguments:

  --help, -h    : Display the help message
  --count, -c   : Number of times to process the input
  --verbose, -v : Enable verbose logging output
```

**Passing valid arguments:**

```text
> ./my-program data.txt -c 5 --verbose
Input path  : data.txt
Count       : 5
Verbose mode: true
```

**Parsing invalid/incorrect arguments:**

```text
> ./my-program --verbose
[argon::error] Missing value for a required argument [input]
... (prints help message)
```

---

Now that you have your parser up and running, you can move on to **[Parser Configuration](https://www.google.com/search?q=parser_cfg.md)** to learn how to heavily customize the parser's behavior, or jump straight into **[Defining Arguments](https://www.google.com/search?q=arguments.md)** to explore the library's vast type support and validation tools.
