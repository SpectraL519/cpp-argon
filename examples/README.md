# CPP-ARGON Examples

A collection of example projects demonstrating the features of the [CPP-ARGON](https://github.com/SpectraL519/cpp-argon) library.

<br />

> [!NOTE]
>
> Each example project has its own README file with detailed explanations.

<br />

## Example Projects

| Project | Description |
|---------|-------------|
| [Power Calculator](power_calculator/) | Calculates the value of a $b^e$ expression for the given base and exponents.<br/>**Demonstrates:** The basic usage of positional and optional arguments. |
| [File Merger](file_merger/) | Merges multiple text files into a single output file.<br/>**Demonstrates:** The usage of default arguments. |
| [Numbers Converter](numbers_converter/) | Converts numbers between different bases.<br/>**Demonstrates:** The usage of argument parameters such as *nargs*, *choices*, and *default values*. |
| [Verbosity](verbosity/) | Prints messages with varying levels of verbosity.<br/>**Demonstrates:** The usage of `none_type` arguments and compound argument flags. |
| [Logging Mode](logging_mode/) | Logs a message depending on the selected logging mode (`quiet`, `normal`, `verbose`).<br/>**Demonstrates:** The usage of custom argument value types (like enums). |
| [Message Logger](message_logger/) | Outputs a message to a file, console, or not at all.<br/>**Demonstrates:** The usage of argument groups. |
| [ARGON-GIT](argon_git/) | A minimal Git CLI clone with subcommands (`init`, `add`, `commit`, `status`, `push`).<br/>**Demonstrates:** The usage of subparsers for multi-command CLIs and complex argument configurations. |

<br />

## Build and Run

### CMake

To build the examples using CMake from the repository root, enable `BUILD_EXAMPLES`:

```shell
cmake -B build -DBUILD_EXAMPLES=ON
cmake --build build/ # -j <njobs>
```

This will generate a `run` executable in each of the `build/examples/<example-name>` directories:

```shell
./build/examples/<example-name>/run <command-line args>
```

> [!NOTE]
>
> Each example project is registered using the `make_example` macro defined in [CMakeLists.txt](CMakeLists.txt).

<br />

### Bazel

Alternatively, you can build all examples with [Bazel](https://bazel.build/) from the repository root:

```shell
bazel build //examples:all_examples
```

To execute a specific example program:

```shell
bazel run //examples/<example-name>:<example-name> -- <command-line args>
```

> [!IMPORTANT]
> Building with Bazel requires having Bzlmod enabled.
> **NOTE:** For Bazel >= 7.0.0, Bzlmod is enabled by default.
