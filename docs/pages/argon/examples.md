# Examples

The library usage examples are included directly in the `examples/` directory of the repository. Each example is a standalone program demonstrating specific features of `CPP-ARGON`.

For detailed information about each project, refer to its individual `README.md` file in the corresponding directory.

---

## Example Projects

The following table lists the example projects provided in the repository:

| Project | Description |
| :- | :- |
| [Power Calculator](https://github.com/SpectraL519/cpp-argon/tree/master/examples/power_calculator/) | Calculates the value of a $b^e$ expression for the given base and exponents.<br/>**Demonstrates:** The basic usage of positional and optional arguments. |
| [File Merger](https://github.com/SpectraL519/cpp-argon/tree/master/examples/file_merger/) | Merges multiple text files into a single output file.<br/>**Demonstrates:** The usage of default arguments. |
| [Numbers Converter](https://github.com/SpectraL519/cpp-argon/tree/master/examples/numbers_converter/) | Converts numbers between different bases.<br/>**Demonstrates:** The usage of argument parameters such as *nargs*, *choices*, and *default values*. |
| [Verbosity](https://github.com/SpectraL519/cpp-argon/tree/master/examples/verbosity/) | Prints messages with varying levels of verbosity.<br/>**Demonstrates:** The usage of `none_type` arguments and compound argument flags. |
| [Logging Mode](https://github.com/SpectraL519/cpp-argon/tree/master/examples/logging_mode/) | Logs a message depending on the selected logging mode (`quiet`, `normal`, `verbose`).<br/>**Demonstrates:** The usage of custom argument value types (like enums). |
| [Message Logger](https://github.com/SpectraL519/cpp-argon/tree/master/examples/message_logger/) | Outputs a message to a file, console, or not at all.<br/>**Demonstrates:** The usage of argument groups. |
| [ARGON-GIT](https://github.com/SpectraL519/cpp-argon/tree/master/examples/argon_git/) | A minimal Git CLI clone with subcommands (`init`, `add`, `commit`, `status`, `push`).<br/>**Demonstrates:** The usage of subparsers for multi-command CLIs and complex argument configurations. |

---

## Building and Running

### CMake

To build all examples with CMake, enable the `BUILD_EXAMPLES` option when configuring the project from the repository root:

```bash
cmake -B build -DBUILD_EXAMPLES=ON
cmake --build build/ # -j<njobs>
```

This generates executable files named `run` in each `build/examples/<project_name>/` directory. To run an example, execute its binary:

```bash
./build/examples/<project_name>/run [arguments]
```

### Bazel

Alternatively, you can build all examples with [Bazel](https://bazel.build/) from the repository root:

```bash
bazel build //examples:all_examples
```

To run a specific example target directly with Bazel:

```bash
bazel run //examples/<project_name>:<project_name> -- [arguments]
```
