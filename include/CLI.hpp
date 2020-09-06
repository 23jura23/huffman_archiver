#ifndef _INCLUDE_CLI_HPP
#define _INCLUDE_CLI_HPP

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace CLI {
class CLI {
 public:
    CLI(int argc, std::vector<std::string> argv);
    std::string getInputFilename();
    std::string getOutputFilename();
    enum class Mode { COMPRESS, EXTRACT };
    Mode getMode();

 private:
    std::optional<std::string> inputFile;
    std::optional<std::string> outputFile;
    std::optional<Mode> mode;
};  // class CLI

struct CLIException : std::runtime_error {
    using std::runtime_error::runtime_error;
    using std::runtime_error::what;
};

};  // namespace CLI

#endif  // _INCLUDE_CLI_HPP
