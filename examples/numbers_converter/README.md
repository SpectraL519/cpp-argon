# CPP-ARGON demo: numbers converter

This demo project demonstrates how you can use an argument's parameters like *nargs*, *choices* or *default values*

<br />

### Functionality

- The program prints all the parsed numbers converted to the selected system.

- The program takes two optional arguments:

  - `numbers` - a list of unsigned integer values (empty list by default).
  - `base` - the number system's base to which the given numbers will be converted. This argument can have exactly one value, which can be one of: `bin`, `dec`, `hex`, where `dec` is the default value.

<br />

### Experiments

Check what happens when you run the `convert_numbers` executable with:

- no arguments
- only the *numbers flag* with values
- the *numbers flag* with values and the *base flag* with one of the values: `bin`, `dec`, `hex`
- the *numbers flag* with values and the *base flag* with a different value

> [!NOTE]
>
> - The *base flag* is either `-b` or `--base`
> - The *numbers flag* is either `-n` or `--numbers`

<br />

### Example program execution

```shell
> ./numbers_converter/run --numbers 10 20 44 --base hex
10 = (a)_16
20 = (14)_16
44 = (2c)_16
```
