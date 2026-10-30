#include <argon/argument_parser.hpp>

enum class logging_mode : uint16_t { quiet, normal, verbose };

std::istream& operator>>(std::istream& is, logging_mode& mode) {
    char mode_value;
    is >> mode_value;

    switch (mode_value) {
    case 'q':
        mode = logging_mode::quiet;
        break;
    case 'n':
        mode = logging_mode::normal;
        break;
    case 'v':
        mode = logging_mode::verbose;
        break;
    default:
        throw std::ios::failure(
            std::format("Invalid logging-mode discrimiator value: {}", mode_value)
        );
    }

    return is;
}

void print_welcome_message(const logging_mode mode) {
    switch (mode) {
    case logging_mode::quiet:
        std::cout << "Hi!" << std::endl;
        break;

    case logging_mode::normal:
        std::cout << "Welcome to CPP-ARGON demo: logging-mode!" << std::endl;
        break;

    case logging_mode::verbose:
        std::cout
            << "Welcome to the CPP-ARGON demo project: logging-mode!" << std::endl
            << "This project demostrates how to use the "
               "argon::argument_parser class with custom types (like enums)"
            << std::endl;
        break;
    }
}

int main(int argc, char** argv) {
    argon::argument_parser parser("logging-mode");
    parser.description("Logs a welcome message based on the selected logging mode")
        .default_arguments(argon::default_argument::o_help);

    parser.add_optional_argument<logging_mode>("logging-mode", "l")
        .default_values(logging_mode::quiet)
        .implicit_values(logging_mode::normal)
        .nargs(1);

    parser.try_parse_args(argc, argv);

    print_welcome_message(parser.value<logging_mode>("logging-mode"));

    return 0;
}
