# Dev Notes

---

## Building and Testing

> [!NOTE]
>
> The project uses the [doctest](https://github.com/doctest/doctest) framework for unit testing, however it is already installed in the `tests/external/` directory, so there is no need to install it separately.

### Build the Testing Executable

```shell
cmake -B build -DBUILD_TESTS=ON
cmake --build build/ # -j<njobs>
```

This will build a `run_uts` executables in the `build/tests/` directory.

### Running the Tests

To run the project's unit tests, simply execute the testing binary:

```shell
./build/tests/run_uts
```

> [!TIP]
>
> Each test file defines its unique test suite with the same name as the test file. To run tests within a specific test suite, execute:
> ```shell
> ./build/tests/run_uts -ts=<test-suite-name>
> ```

---

## Formatting

> [!NOTE]
>
> The project uses `clang-format-18` which is natively available on the `Ubuntu-24.04 LTS` system.
>
> If the `llvm-18` toolchain isn't available on your system, the installation instructions can be found on the [llvm apt](https://apt.llvm.org/) page. After installing the toolchain run `sudo apt install clang-format-18`.

You can format the code manually using `clang-format-18` or you can use the prepared python script:

```shell
python scripts/format.py
```

To inspect all the script's options and usage, run:

```shell
python scripts/format.py --help
```

> [!NOTE]
>
> The script requires `python >= 3.9`.

---

## Documentation

> [!NOTE]
>
> You can view the online documentation for the latest version [here](https://spectral519.github.io/cpp-argon/latest/).
> To view the documentation for previous versions, use the following URL pattern:
> ```txt
> [https://spectral519.github.io/cpp-argon/](https://spectral519.github.io/cpp-argon/)<version>
> ```

The documentation for this project is built using a modern toolchain combining:

- **Doxygen** (to parse C++ sources into XML), 
- **MkDocs** (with the Material theme and `mkdoxy` plugin to generate the site), 
- **mike** (to handle versioning).

### Prerequisites

1. **Doxygen**: Ensure Doxygen is installed on your system. For example, on Ubuntu:

```shell
sudo apt update && sudo apt install doxygen -y
```

2. **uv**: Install the `uv` Python package manager:

```shell
pip install uv
```

### Setup

Before building the documentation for the first time, install and sync the required Python dependencies defined in `pyproject.toml`:

```shell
uv sync
```

### Building and Serving Locally

The project includes a `Makefile` to completely automate the documentation generation and deployment process.

To build the documentation, deploy it to your local Git history under the `dev` alias, and serve it with live-reloading, run:

```shell
make clean
make docs # uses TAGS="dev" by default
make serve-docs
```

This will start a local server. You can view the documentation by navigating your browser to `http://localhost:8000/dev/`.

### Makefile Targets

Here are the available Make targets for managing the documentation:

- `make build-docs`: Builds the documentation directly into the `site/` directory without deploying it (useful for quick CI validations).
- `make docs TAGS="<tags>"`: Builds and deploys the documentation to the local `gh-pages` branch using `mike`.
- `make serve-docs`: Starts the local `mike` web server.
- `make clean` (or `make clean-docs`): Removes all generated build directories (`docs/cpp-argon/`, `site/`, and `documentation/`).
