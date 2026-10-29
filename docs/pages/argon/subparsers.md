# Subparsers

Subparsers allow you to build **hierarchical command-line interfaces**, where a top-level parser delegates parsing to its subcommands. This is particularly useful for creating CLI applications like `git`, where commands such as `git add`, `git commit`, and `git push` each have their own independent arguments.

---

## Creating Subparsers

Each subparser is a separate instance of `argon::argument_parser`. Therefore, it can have its own parameters, including a description, custom arguments, argument groups, and even its own nested subparsers.

You can create a subparser by calling the `add_subparser` method on a parent parser:

```cpp
argon::argument_parser git("argon-git"); // (1)!
git.version("v0.1.0")
   .description("A version control system built with CPP-ARGON")
   .default_arguments(argon::default_argument::o_help, argon::default_argument::o_version);

auto& status = git.add_subparser("status"); // (2)!
status.default_arguments(argon::default_argument::o_help)
      .description("Show the working tree status");

status.add_flag("short", "s") // (3)!
      .help("Give the output in the short-format");
```

1. Initialize the top-level parser.
2. Create a new subparser named `"status"`. This returns a reference to the newly created `argument_parser` instance.
3. Configure arguments directly on the subparser.

---

## Using Multiple Subparsers

You can add as many subparsers as you like, each corresponding to a different command:

```cpp
auto& init = git.add_subparser("init");
init.description("Create an empty Git repository or reinitialize an existing one");

auto& add = git.add_subparser("add");
add.description("Add file contents to the index");

auto& commit = git.add_subparser("commit");
commit.description("Record changes to the repository");

auto& status = git.add_subparser("status");
status.description("Show the working tree status");

auto& push = git.add_subparser("push");
push.description("Update remote refs along with associated objects");
```

All defined subparsers will automatically be grouped and included in the parent parser's generated help message:

```txt
> argon-git --help
Program: argon-git v0.1.0

  A version control system built with CPP-ARGON

Commands:

  init   : Create an empty Git repository or reinitialize an existing one
  add    : Add file contents to the index
  commit : Record changes to the repository
  status : Show the working tree status
  push   : Update remote refs along with associated objects

Optional Arguments:

  --help, -h    : Display the help message
  --version, -v : Display program version info
```

---

## Command-Line Delegation

When parsing command-line arguments, the parent parser will attempt to match the **first command-line token** against the name of one of its subparsers.

- If a match is found, the parser immediately delegates the remaining arguments to the matched subparser.
- This process repeats **recursively**, meaning each subparser may also match one of its own nested subparsers.
- Parsing stops delegating when no subparser matches the first token of the *current* argument list. At that point, the active parser processes its own arguments.

For example:

```cpp
argon::argument_parser git("argon-git");
auto& submodule = git.add_subparser("submodule"); // (1)!
auto& submodule_init = submodule.add_subparser("init"); // (2)!
```

1. First-level subparser.
2. Second-level (nested) subparser.

Running `argon-git submodule init <args>` will result in `<args>` being parsed entirely by the `submodule_init` parser.

---

## Tracking Parser State

Because parsing can be delegated through multiple layers of subparsers, `CPP-ARGON` tracks the state of each parser during execution. You can inspect this state using the following methods:

- `invoked() -> bool`

    Returns `true` if the parser’s name appeared on the command line. A parser becomes *invoked* as soon as it is selected during parsing, even if parsing is immediately delegated to one of its subparsers.

- `finalized() -> bool`

    Returns `true` if the parser successfully processed its own arguments. This is distinct from `invoked()`: a parser can be *invoked* but not *finalized* if one of its subparsers handled the trailing arguments instead.

- `resolved_parser() -> argon::argument_parser&`

    Returns a reference to the *deepest invoked parser*. If no subparser was invoked, this simply returns the current parser.

### Example: Inspecting Parsing States

```cpp
argon::argument_parser git("argon-git");
auto& submodule = git.add_subparser("submodule");
auto& submodule_init = submodule.add_subparser("init");

git.try_parse_args(argc, argv); // (1)!

std::cout << std::boolalpha;

std::cout << "git            : invoked=" << git.invoked() // (2)!
          << ", finalized=" << git.finalized() << '\n';

std::cout << "submodule      : invoked=" << submodule.invoked()
          << ", finalized=" << submodule.finalized() << '\n';

std::cout << "submodule_init : invoked=" << submodule_init.invoked()
          << ", finalized=" << submodule_init.finalized() << '\n';

auto& active = git.resolved_parser(); // (3)!
std::cout << "\nResolved parser : " << active.name() << " (" << active.full_name() << ")\n";
```

1. Parse the command-line inputs.
2. Inspect the invocation and finalization state of each parser in the hierarchy.
3. Retrieve the deepest parser that was actually evaluated.

If you run the program with `./argon-git submodule init`, you will get the following state output:

```txt
git            : invoked=true, finalized=false
submodule      : invoked=true, finalized=false
submodule_init : invoked=true, finalized=true

Resolved parser : init (argon-git submodule init)
```
