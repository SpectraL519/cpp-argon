#include <argon/argument_parser.hpp>

#include <fstream>

int main(int argc, char** argv) {
    argon::argument_parser parser("message-logger");
    parser.description("Logs a given message based on the selected output mode")
        .default_arguments(argon::default_argument::o_help);

    // Output mode selector arguments - exactly one argument must be used
    auto& out_mode = parser.add_group("Output Mode")
        .description("Defines the possible output (logging) modes - exactly one must be selected")
        .required()
        .mutually_exclusive();
    parser.add_optional_argument(out_mode, "file", "f")
        .nargs(1)
        .help("Write the message into a given file");
    parser.add_flag(out_mode, "console", "c").help("Print the message to the console");
    parser.add_flag(out_mode, "silent", "s").help("Do not output anything");

    // Additional message options
    auto& msg_opts = parser.add_group("Message Options");
    parser.add_optional_argument(msg_opts, "message", "m")
        .nargs(1)
        .default_values("Hello from CPP-ARGON demo: message-logger!")
        .help("Specify a custom message");

    parser.try_parse_args(argc, argv);

    const std::string& msg = parser.value("message");

    if (parser.has_value("file")) {
        std::ofstream out(parser.value("file"));
        out << msg << std::endl;
        std::cout << "Message written to file: " << parser.value("file") << std::endl;
    }
    else if (parser.value<bool>("console")) {
        std::cout << msg << std::endl;
    }
    else if (parser.value<bool>("silent")) {
        // do nothing
    }
    else {
        std::cout << "No output mode selected!" << std::endl;
        return 1;
    }

    return 0;
}
