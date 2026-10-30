# CPP-ARGON-DEMO

A collection of the [CPP-ARGON](https://github.com/SpectraL519/cpp-argon) library demo projects.

<br />

> [!NOTE]
>
> Each demo project has it's own README file with it's detailed explanation

<br />

## Demo Projects

| Project | Description |
|---------|-------------|
| [Power Calculator](/power_calculator/) | Calculates the value of a $b^e$ expression for the given base and exponents.<br/>**Demonstrates:** The basic usage of positional and optional arguments. |
| [File Merger](/file_merger/) | Merges multiple text files into a single output file.<br/>**Demonstrates:** The usage of default arguments. |
| [Numbers Converter](/numbers_converter/) | Converts numbers between different bases.<br/>**Demonstrates:** The usage of argument parameters such as *nargs*, *choices*, and *default values*. |
| [Verbosity](/verbosity/) | Prints messages with varying levels of verbosity.<br/>**Demonstrates:** The usage of `none_type` arguments and compound argument flags. |
| [Logging Mode](/logging_mode/) | Logs a message depending on the selected logging mode (`quiet`, `normal`, `verbose`).<br/>**Demonstrates:** The usage of custom argument value types (like enums). |
| [Message Logger](/message_logger/) | Outputs a message to a file, console, or not at all.<br/>**Demonstrates:** The usage of argument groups. |
| [ARGON-GIT](/argon_git/) | A minimal Git CLI clone with subcommands (`init`, `add`, `commit`, `status`, `push`).<br/>**Demonstrates:** The usage of subparsers for multi-command CLIs and complex argument configurations. |


<br />

## Build and Run

To build the demo projects using CMake run the following:

```shell
cmake -B build # -DAP_TAG=<AP_TAG>
cmake --build build/ # -j <njobs>
```

This will generate a `run` executable in each of the `build/<demo-project-name>` directories, so to execute the built programs you can run:

```shell
./build/<demo-project-name>/run <command-line args>
```

> [!NOTE]
>
> Each project is built in the exact same way - using the `make_example` macro defined in the root [CMakeLists.txt](/CMakeLists.txt) file

<br />

Alternatively you can use [Bazel](https://bazel.build/):

```shell
bazel build //:all_demos
```

This generate a target for each of the demo projects. To execute the built programs run:

```shell
bazel run //<demo-project-name>:<demo-project-name> -- <command-line args>
```

> [!IMPORTANT]
> Building with bazel requires having Bzlmod enabled.
> **NOTE:** For newer versions (>= 7.0.0) Bzlmod is enabled by default.
