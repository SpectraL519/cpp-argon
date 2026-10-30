#include <argon/argument_parser.hpp>

int main(int argc, char* argv[]) {
    // top-level git parser
    argon::argument_parser git("argon-git");
    git.version("v0.1.0")
        .description("A minimal Git CLI clone built with CPP-ARGON")
        .default_arguments(argon::default_argument::o_help, argon::default_argument::o_version);

    // subcommand: init
    auto& init =
        git.add_subparser("init")
            .default_arguments(argon::default_argument::o_help)
            .description("Create an empty Git repository or reinitialize an existing one");
    init.add_optional_argument<argon::none_type>("bare").help("Create a bare repository");

    // subcommand: add
    auto& add =
        git.add_subparser("add")
            .default_arguments(argon::default_argument::o_help)
            .description("Add file contents to the index");
    auto& add_pathspec_args = add.add_group("Pathspec Arguments").required();
    add.add_positional_argument(add_pathspec_args, "pathspec")
        .required(false)
        .nargs(argon::nargs::any())
        .help("Files to add");
    add.add_flag(add_pathspec_args, "update", "u")
        .help("Update the index just where it already has an entry matching <pathspec>");

    // subcommand: commit
    auto& commit =
        git.add_subparser("commit")
            .default_arguments(argon::default_argument::o_help)
            .description("Record changes to the repository");
    commit.add_optional_argument("message", "m")
        .nargs(1)
        .help("Use the given message as the commit message");
    commit.add_flag("all", "a")
        .help("Automatically stage files that have been modified and deleted");
    commit.add_flag("amend").help("Replace the tip of the current branch by creating a new commit");

    // subcommand: status
    auto& status =
        git.add_subparser("status")
            .default_arguments(argon::default_argument::o_help)
            .description("Show the working tree status");
    status.add_flag("short", "s").help("Give the output in the short-format");

    // subcommand: push
    auto& push =
        git.add_subparser("push")
            .default_arguments(argon::default_argument::o_help)
            .description("Update remote refs along with associated objects");
    push.add_positional_argument("remote").required(false).default_values("origin").help(
        "Remote repository (e.g. origin)"
    );
    push.add_positional_argument("branch").required(false).default_values("main").help(
        "Branch to push (e.g. main)"
    );

    // Parse and inspect results
    git.try_parse_args(argc, argv);

    if (init.finalized()) {
        std::cout << "Initializing " << (init.is_used("bare") ? "bare " : "") << "repository.."
                  << std::endl;
        return 0;
    }

    if (add.finalized()) {
        const auto& pathspec = add.values("pathspec");
        if (add.value<bool>("update")) {
            std::cout << "Updating index at matching pathspec";
            if (not pathspec.empty())
                std::cout << " from paths: " << argon::util::join(pathspec);
            std::cout << std::endl;
        }
        else if (not pathspec.empty()) {
            std::cout << "Adding files from pathspec: " << argon::util::join(pathspec) << std::endl;
        }
        else {
            std::cout << "No pathspec specified. Nothing to add." << std::endl;
        }
        return 0;
    }

    if (commit.finalized()) {
        std::cout << "Committing";

        if (commit.value<bool>("all"))
            std::cout << " (all tracked files staged)";

        if (not commit.has_value("message"))
            std::cout << " with message: \"" << commit.value("message") << "\"";

        if (commit.value<bool>("amend"))
            std::cout << " (amending last commit)";

        std::cout << std::endl;
        return 0;
    }


    if (status.finalized()) {
        if (status.value<bool>("short"))
            std::cout << "On branch main\nM file1.cpp\n?? file2.cpp\n";
        else
            std::cout << "On branch main\nnothing to commit, working tree clean\n";
        return 0;
    }

    if (push.finalized()) {
        std::cout << "Pushing branch " << push.value("branch") << " to " << push.value("remote")
                  << std::endl;
        return 0;
    }

    return 0;
}
