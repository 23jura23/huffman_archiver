#include "CLI.hpp"

namespace CLI {

constexpr const char* COMPRESS_OPTION = "-c";
constexpr const char* UNCOPMRESS_OPTION = "-u";
constexpr const char* INPUT_FILE_OPTIONS[] = {"-f", "--file"};
constexpr const char* OUTPUT_FILE_OPTIONS[] = {"-o", "--output"};

CLI::CLI(int argc, std::vector<std::string> argv) {
    auto WrongArguments = CLIException("Arguments are incorrect");
    if (argc != 6 || argv.size() != static_cast<size_t>(argc)) {
        throw WrongArguments;
    }
    for (int i = 1; i < argc; ++i) {
        if (!mode.has_value() && argv[i] == COMPRESS_OPTION) {
            mode = Mode::COMPRESS;
            continue;
        }
        if (!mode.has_value() && argv[i] == UNCOPMRESS_OPTION) {
            mode = Mode::EXTRACT;
            continue;
        }
        if (i < argc - 1 && !inputFile.has_value()) {
            for (auto option : INPUT_FILE_OPTIONS) {
                if (argv[i] == option) {
                    inputFile = argv[i + 1];
                    i++;
                    break;
                }
            }
            if (inputFile.has_value())
                continue;
        }
        if (i < argc - 1 && !outputFile.has_value()) {
            for (auto option : OUTPUT_FILE_OPTIONS) {
                if (argv[i] == option) {
                    outputFile = argv[i + 1];
                    i++;
                    break;
                }
            }
            if (outputFile.has_value())
                continue;
        }
        throw WrongArguments;
    }
    if (!mode.has_value() || !inputFile.has_value() || !outputFile.has_value())
        throw WrongArguments;
}

std::string CLI::getInputFilename() {
    return inputFile.value();
}

std::string CLI::getOutputFilename() {
    return outputFile.value();
}

CLI::Mode CLI::getMode() {
    return mode.value();
}
}  // namespace CLI
