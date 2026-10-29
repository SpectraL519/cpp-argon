# Examples

The library usage examples and demo projects are included in the `cpp-argon-demo` submodule.
To fetch the submodule content after cloning the main repository, run:

```bash
git submodule update --init --recursive
```

For more detailed information about the demo projects, see the [cpp-argon-demo](https://github.com/SpectraL519/cpp-argon-demo) README.

The following table lists the projects provided in the `cpp-argon-demo` submodule:

| Project | Description |
| :- | :- |
| [Power Calculator](https://github.com/SpectraL519/cpp-argon-demo/tree/master/power_calculator/) | Calculates the value of a $b^e$ expression for the given base and exponents.<br/>**Demonstrates:** The basic usage of positional and optional arguments. |
| [File Merger](https://github.com/SpectraL519/cpp-argon-demo/tree/master/file_merger/) | Merges multiple text files into a single output file.<br/>**Demonstrates:** The usage of default arguments. |
| [Numbers Converter](https://github.com/SpectraL519/cpp-argon-demo/tree/master/numbers_converter/) | Converts numbers between different bases.<br/>**Demonstrates:** The usage of argument parameters such as *nargs*, *choices*, and *default values*. |
| [Verbosity](https://github.com/SpectraL519/cpp-argon-demo/tree/master/verbosity/) | Prints messages with varying levels of verbosity.<br/>**Demonstrates:** The usage of `none_type` arguments and compound argument flags. |
| [Logging Mode](https://github.com/SpectraL519/cpp-argon-demo/tree/master/logging_mode/) | Logs a message depending on the selected logging mode (`quiet`, `normal`, `verbose`).<br/>**Demonstrates:** The usage of custom argument value types (like enums). |
| [Message Logger](https://github.com/SpectraL519/cpp-argon-demo/tree/master/message_logger/) | Outputs a message to a file, console, or not at all.<br/>**Demonstrates:** The usage of argument groups. |
| [ARGON-GIT](https://github.com/SpectraL519/cpp-argon-demo/tree/master/argon_git/) | A minimal Git CLI clone with subcommands (`init`, `add`, `commit`, `status`, `push`).<br/>**Demonstrates:** The usage of subparsers for multi-command CLIs and complex argument configurations. |
