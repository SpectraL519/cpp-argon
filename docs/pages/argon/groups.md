# Argument Groups

Argument groups provide a way to organize related arguments into logical sections and enforce group-wise requirements.

By default, every parser comes with two predefined groups:

- **Positional Arguments** – contains all arguments added via `add_positional_argument`.
- **Optional Arguments** – contains all arguments added via `add_optional_argument` or `add_flag`.

> [!IMPORTANT]
>
> All [default arguments](arguments.md#default-arguments) are bound to one of the predefined groups.

---

## Creating New Groups

A new group can be created by calling the `add_group` method of an argument parser:

```cpp
argon::argument_parser parser("myprog");
auto& out_opts = parser.add_group("Output Options"); // (1)!
```

1. Creates a new argument group with the specified name.

The group’s name will appear as a dedicated section in the help message and arguments added to this group will be listed under `Output Options` instead of the default `Optional Arguments` section.

> [!NOTE]
>
> If a group has no visible arguments, it will not be included in the parser's help message output at all.

---

## Adding Arguments to Groups

Arguments are added to a group by passing the group reference as the first parameter to the one of the argument adding functions (i.e., `add_positional_argument`, `add_optional_argument`, `add_flag`):

```cpp
parser.add_optional_argument(out_opts, "output", "o") // (1)!
      .nargs(1)
      .help("Print output to the given file");

parser.add_flag(out_opts, "print", "p") // (2)!
      .help("Print output to the console");
```

1. Add an optional argument to the `out_opts` group.
2. Add a flag to the `out_opts` group.

---

## Group Attributes

User-defined groups can be configured with special attributes that change how the parser enforces their usage, modifies their arguments' names, or handles their visibility and descriptions in the help output:

### Validation Rules

- `required()` – at least one argument from the group must be provided by the user, otherwise parsing will fail.
- `mutually_exclusive()` – at most one argument from the group can be provided; using more than one at the same time results in an error.

Both attributes are **off by default**

```cpp
auto& out_opts = parser.add_group("Output Options")
                       .required()            // (1)!
                       .mutually_exclusive(); // (2)!
```

> [!TIP]
> 
> The `required` and `mutually_exclusive` attributes can be combined (i.e., a group can require that exactly one argument is chosen).

1. At least one option from the group is required.
2. At most one option from the group can be chosen.

> [!IMPORTANT]
>
> If a group is defined as **mutually exclusive** and an argument from this group is used, then the `required` and `nargs` attribute requirements of other arguments from the group **will NOT be verified**.
>
> Consider the program in the [Complete Example](#complete-example) section below. Normally the `--out-file, -out-f` argument would expect a value to be given in the command-line due to the `.nargs(1)` parameter. However, if the `--out-console, -out-c` flag is used, then the `nargs` requirement of the `--out-file, -out-f` argument will not be verified, and therefore no exception will be thrown, even though the `nargs` requirement is not satisfied.

### Naming Modifiers

Groups can automatically apply modifiers to the names of all arguments registered to them. This is especially useful for preventing name collisions or grouping related arguments under a common namespace.

- `with_prefix("str")` – prepends the specified string to the argument's base name.
- `with_suffix("str")` – appends the specified string to the argument's base name.

```cpp
auto& net_opts = parser.add_group("Network Options")
                       .with_prefix("net-")
                       .with_suffix("-cfg");

net_opts.add_optional_argument("port"); // (1)!
```

1. Registered in the main parser as `"net-port-cfg"`.

> [!NOTE]
>
> When using naming modifiers, the argument is registered in the main parser under its fully modified name. To learn how to easily retrieve values using only the base names, see [Retrieving Values from Groups](TODO).

### Description

You can attach a text description to an argument group to provide extra context to the user. This description will be printed immediately underneath the group's name and requirements in the generated help text.

```cpp
auto& auth_opts = parser.add_group("Authentication")
                        .description("Provide credentials to access the remote server.");
```

### Visibility

- `hidden()` – If this option is set, the entire group (including all of its arguments) will be hidden from the program's help description.

  ```cpp
  auto& hidden_opts = parser.add_group("Hidden Options").hidden();
  parser.add_optional_argument(hidden_opts, "visible").help("A visible arg"); // (1)!
  ```

  1. This argument will not be printed in the parser's help output because its parent group is hidden.

---

## Inspecting Group Attributes

If you need to dynamically query a group's configuration at runtime (e.g., when generating a GUI wrapper or a shell auto-completion script), you can use the group's attribute getters:

```cpp
const std::string& name = group.name();
const std::string& desc = group.description();

bool is_hidden   = group.is_hidden();
bool is_required = group.is_required();
bool is_mutex    = group.is_mutually_exclusive();

const std::string& prefix = group.prefix();
const std::string& suffix = group.suffix();
```

---

## Complete Example

Below is a small program that demonstrates how to use a mutually exclusive group of required arguments:

```cpp
#include <argon/argument_parser.hpp>

int main(int argc, char* argv[]) {
    argon::argument_parser parser("myprog");
    parser.default_arguments(argon::default_argument::o_help);

    auto& out_opts = parser.add_group("Output Options") // (1)!
                           .description("Select exactly one destination for the output.")
                           .with_prefix("out-")
                           .required()
                           .mutually_exclusive();

    parser.add_optional_argument(out_opts, "file", "f") // (2)!
          .nargs(1)
          .help("Print output to a given file");

    parser.add_flag(out_opts, "console", "c") // (3)!
          .help("Print output to the console");

    parser.try_parse_args(argc, argv);

    return 0;
}
```

1. Create the argument group.
2. Add an argument to the custom group.
3. Add a flag to the custom group.

When invoked with the `--help` flag, the above program produces a help message that clearly shows the group, its description, and its rules:

```text
Program: myprog

Optional Arguments:

  --help, -h : Display the help message

Output Options: (required, mutually exclusive)

  Select exactly one destination for the output.

  --out-file, -out-f    : Print output to a given file
  --out-console, -out-c : Print output to the console
```

---

## Suppressing Argument Group Checks

Similarly to [suppressing argument checks](arguments.md#argument-checks-suppression--suppress_arg_checks), an argument can suppress the requirement checks of argument groups:

```cpp
argument.suppress_group_checks();
```

If such an argument is used, the requirement checks associated with attributes of **all parser's groups** will not be validated.

> [!WARNING]
>
> - Enabling the `suppress_group_checks` option has no effect on individual argument requirements validation.
> - Enabling argument group checks suppression is not possible for required arguments (an exception will be thrown).
