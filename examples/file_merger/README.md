# CPP-ARGON demo: file merger

This demo project demonstrates how you can use the default arguments.

The program takes two default optional arguments:

- `input` - a list of file paths to be merged with a default action: `check_file_exists`
- `output` - output file path

Both of these arguments are defined using the `parser.default_arguments` method.

<br />

### Functionality

The program merges all input files (in the same order they are passed to the program) into a single output file.

<br />

### Experiments

Check what happens when you run the program with:

- no arguments
- only the *input flag* with values / only the *output flag* with values
- both the *input* and *output* flags with values

> [!NOTE]
>
> - The *input flag* is either `-i` or `--input`
> - The *output flag* is either `-o` or `--output`

<br />

### Example program execution

```shell
> ./file_merger/run -i data/test_file_1.txt data/test_file_2.txt -o out_file.txt
```
