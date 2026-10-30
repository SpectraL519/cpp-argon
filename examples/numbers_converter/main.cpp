#include <argon/argument_parser.hpp>

#include <bitset>
#include <iostream>
#include <sstream>

void convert_to_binary(const std::vector<std::size_t>& numbers) {
    constexpr std::size_t nbits = sizeof(std::size_t) * 8;
    for (const auto n : numbers)
        std::cout << n << " = (" << std::bitset<nbits>(n) << ")_2" << std::endl;
}

void convert_to_decimal(const std::vector<std::size_t>& numbers) {
    for (const auto n : numbers)
        std::cout << n << " = (" << n << ")_10" << std::endl;
}

void convert_to_hex(const std::vector<std::size_t>& numbers) {
    std::stringstream ss;
    ss << std::hex;
    for (const auto n : numbers) {
        ss << n;
        std::cout << n << " = (" << ss.str() << ")_16" << std::endl;
        ss.str(std::string());
    }
}

void convert_numbers(const std::vector<std::size_t>& numbers, std::string_view base) {
    if (base == "bin")
        convert_to_binary(numbers);
    else if (base == "hex")
        convert_to_hex(numbers);
    else
        convert_to_decimal(numbers);
}

int main(int argc, char* argv[]) {
    argon::argument_parser parser("convert-numbers");
    parser.description("Converts given positive integers into a specified base format")
        .default_arguments(argon::default_argument::o_help);

    parser.add_optional_argument<std::size_t>("numbers", "n")
        .nargs(argon::nargs::any())
        .help("positive integer value");
    parser.add_optional_argument("base", "b")
        .nargs(1)
        .default_values("dec")
        .choices({"bin", "dec", "hex"})
        .help("output number format base");

    parser.try_parse_args(argc, argv);

    const auto& numbers = parser.values<std::size_t>("numbers");
    const auto base = parser.value("base");

    convert_numbers(numbers, base);

    return 0;
}
