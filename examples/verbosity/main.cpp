#include <argon/argument_parser.hpp>

void print_welcome_message(const std::size_t verbosity) {
    switch (verbosity) {
    case 0ull:
        std::cout << "Hi!" << std::endl;
        break;

    case 1ull:
        std::cout << "Welcome to CPP-ARGON demo: verbosity!" << std::endl;
        break;

    default:
        std::cout << "Welcome to the CPP-ARGON demo project: verbosity!" << std::endl
                  << "This project demostrates how to use compound argument flags" << std::endl;
        break;
    }
}

int main(int argc, char** argv) {
    argon::argument_parser parser("verbosity");
    parser.description("Prints a welcome message with different verbosity levels")
        .default_arguments(argon::default_argument::o_help);

    parser.add_optional_argument<argon::none_type>("verbose", "v");

    parser.try_parse_args(argc, argv);

    print_welcome_message(parser.count("verbose"));

    return 0;
}
