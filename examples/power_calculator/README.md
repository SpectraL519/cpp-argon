# CPP-ARGON demo: power calculator

This demo project demonstrates how to add positional and optional arguments to the parser's configuration.

<br />

### Functionality

- The program calculates the value of an `<base> ^ <exp>` expression for a single base value and multiple exponents.

- The program takes two arguments:

  - (positional) `base` - the exponentation base
  - (optional) `exponent` - a list of exponents (specified to take at least one argument)

<br />

### Experiments

Check what happens when you run the `power` executable with:
- no arguments
- only the *base positional value*
- the *base positional value* positional value and the *exponent flag* with at least one value

> [!NOTE]
>
> - The *base positional value* is parsed without any flags, e.g. `./power 2`
> - The *exponent flag* is either `-e` or `--exponent`.

<br />

### Example program execution

```shell
> ./power_calculator/run 3 --exponent 1 2 3
3 ^ 1 = 3
3 ^ 2 = 9
3 ^ 3 = 27
```
