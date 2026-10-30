# CPP-ARGON demo: verbosity

This demo project demonstrates how you can use `none_type` arguments and compound argument flags.

<br />

### Functionality

- The program prints a massege based on the given verbosity level.

- The program takes one optional argument: `--verbose, -v`, which defines the level of verbosity for the printed message. The argument takes no values and the verbosity level is determined by the number of times it's flag has been used.

<br />

### Experiments

Check what happens when you run the `verbosity` executable with:

- no arguments
- only the *primary verbosity flag*: `--verbose`
- multiple *primary verbosity flags*: `--verbose --verbose`
- only the *secondary verbosity flag*: `-v`
- multiple *secondary verbosity flags*: `-v -v`
- a *compound verbosity flag*: `-vv`, `-vvv`

#### Example program execution

```shell
> ./verbosity/run -vv
Welcome to the CPP-ARGON demo project: verbosity!
This project demostrates how to use compound argument flags
```
