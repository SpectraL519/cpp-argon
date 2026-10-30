# CPP-ARGON demo: message-logger

This demo project demonstrates how to bind arguments together using argument groups and define requirements for groupped arguments.

<br />

### Functionality

- The program requires you to select **exactly one output mode** from the following options:
  - `--file, -f <filename>` : write the message into a file
  - `--console, -c` : print the message to the console
  - `--silent, -s` : no output at all

  This is done by defining the arguments listed above within a *required* and *mutually exclusive* group.

- Additionally, you can optionally specify a custom message with `--message, -m <text>`.
  If not provided, a default message will be used.

<br />

### Experiments

Try running the program with:

- the `--help, -h` flag : examine the structure of the printed message
- no arguments
- one of the *output mode flags*
- one of the *output mode flags* and the message flag
- using multiple *output mode flags* (e.g., `-c -s`)

<br />

### Example Program Execution

```shell
> ./message_logger/run -c
Hello from CPP-ARGON demo: message-output!
```
