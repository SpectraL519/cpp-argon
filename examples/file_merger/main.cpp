#include <argon/argument_parser.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char* argv[]) {
    argon::argument_parser parser("merge-files");
    parser.description("Merges multiple input text files into a single output file")
        .default_arguments(
            argon::default_argument::o_help,
            argon::default_argument::o_multi_input,
            argon::default_argument::o_output
        );

    parser.try_parse_args(argc, argv);

    const auto& input_file_name_list = parser.values("input");
    const auto& output_file_name = parser.value("output");

    std::ofstream output_file(output_file_name);
    if (not output_file.is_open())
        throw std::runtime_error("Cannot open file: " + output_file_name);

    for (const auto& input_file_name : input_file_name_list) {
        std::ifstream input_file(input_file_name);
        std::stringstream buff;
        buff << input_file.rdbuf();
        output_file << buff.str();
    }

    return 0;
}
