# CPP-ARGON demo: ARGON-GIT

This demo project demonstrates how to build a **multi-command CLI application** with `argon::argument_parser` using **subparsers**, **argument groups**, and **default arguments**.
It mimics a minimal subset of Git’s command-line interface.

<br />

### Functionality

The program provides several Git-like subcommands:

- **`init`** : create an empty Git repository or reinitialize an existing one
  - `--bare` : create a bare repository

- **`add`** : add file contents to the index
  - `pathspec...` (positional, required) : files to add
  - `--update, -u` : update the index where it already has an entry matching `<pathspec>`

- **`commit`** : record changes to the repository
  - `--message, -m <text>` : use the given message as the commit message
  - `--all, -a` : stage all modified and deleted files
  - `--amend` : amend the last commit

- **`status`** : show the working tree status
  - `--short, -s` : print a short-format status

- **`push`** : update remote refs along with associated objects
  - `[remote]` (default: `origin`) : remote repository
  - `[branch]` (default: `main`) : branch to push

Global options:
- `--help, -h` : print help
- `--version, -v` : print version info (`argon-git 2.43.0`)

<br />

### Experiments

Try running the program with:

- the `--help, -h` flag (global help, or per subcommand help)
- `init`, with and without `--bare`
- `add` with different pathspecs, and with `--update`
- `commit` with various flag combinations (`-m`, `-a`, `--amend`)
- `status`, with and without `-s`
- `push` with no arguments, or with explicit remote/branch names

<br />

### Example Program Execution

```shell
> ./argon_git/run --version
argon-git 2.43.0

> ./argon_git/run init --bare
Initializing bare repository..

> ./argon_git/run add file1.cpp file2.cpp
Adding files from pathspec: file1.cpp file2.cpp

> ./argon_git/run commit -a -m "Fix bug in parser"
Committing (all tracked files staged) with message: "Fix bug in parser"

> ./argon_git/run status -s
On branch main
M file1.cpp
?? file2.cpp

> ./argon_git/run push
Pushing branch main to origin
```
