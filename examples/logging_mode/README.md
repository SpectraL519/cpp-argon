# CPP-ARGON demo: logging-mode

This demo project demonstrates how you can use the `argon::argument_parser` class with custom types (like enums), as well as the `default_values` and `implicit_values` parameters.

<br />

### Functionality

- The program prints a massege based on the selected logging mode.

- The program takes one optional argument: `--logging-mode, -l`. It defines the mode which should be used to log a message within the program. This argument can have values: `q` (quiet), `n` (normal), `v` (verbose). The argument takes such values not because they are specified as it's *choices*, but because the `istream>>` operator defined for the type handles onle these values.

<br />

### Experiments

Check what happens when you run the program with:

- no arguments
- only the *logging mode flag* and no values
- the *logging mode flag* with one of the allowed values: `q`, `n`, `v`
- the *logging mode flag* with a different value

> [!NOTE]
>
> The *logging mode flag* is either `--logging-mode` or `-l`

### Example Program Execution
```shell
> ./logging_mode/run -l n
Welcome to CPP-ARGON demo: logging-mode!
```
