#include <argon/argument_parser.hpp>

#include <cmath>
#include <iostream>

int main(int argc, char* argv[]) {
    // create the parser class instance
    argon::argument_parser parser("power-calculator");
    parser.description("Calculates the value of an expression: base & exponent")
        .default_arguments(argon::default_argument::o_help);

    // add arguments
    parser.add_positional_argument<double>("base").help("the exponentation base value");
    parser.add_optional_argument<int>("exponent", "e")
        .nargs(argon::nargs::at_least(1)) // define the number of values range for the argument
        .help("the exponent value");

    // parse command-line arguments
    parser.try_parse_args(argc, argv);

    const double base = parser.value<double>("base");
    const std::vector<int>& exponent_values = parser.values<int>("exponent");

    for (const int exponent : exponent_values) {
        std::cout << base << " ^ " << exponent << " = " << std::pow(base, exponent) << std::endl;
    }

    return 0;
}
