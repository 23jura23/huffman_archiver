#include <doctest.h>

#include <vector>

#include "CLI.hpp"

std::vector<std::vector<std::string>> allArgvCompress = {  // oh...
    {"hw_03", "-c", "-f", "file1.txt", "-o", "file2.txt"},
    {"hw_03", "-c", "--file", "file1.txt", "-o", "file2.txt"},
    {"hw_03", "-c", "--file", "file1.txt", "--output", "file2.txt"},
    {"hw_03", "-c", "-f", "file1.txt", "--output", "file2.txt"},
    {"hw_03", "-c", "-o", "file2.txt", "-f", "file1.txt"},
    {"hw_03", "-c", "-o", "file2.txt", "--file", "file1.txt"},
    {"hw_03", "-c", "--output", "file2.txt", "--file", "file1.txt"},
    {"hw_03", "-c", "--output", "file2.txt", "-f", "file1.txt"},
    {"hw_03", "-f", "file1.txt", "-c", "-o", "file2.txt"},
    {"hw_03", "--file", "file1.txt", "-c", "-o", "file2.txt"},
    {"hw_03", "--file", "file1.txt", "-c", "--output", "file2.txt"},
    {"hw_03", "-f", "file1.txt", "-c", "--output", "file2.txt"},
    {"hw_03", "-f", "file1.txt", "-o", "file2.txt", "-c"},
    {"hw_03", "--file", "file1.txt", "-o", "file2.txt", "-c"},
    {"hw_03", "--file", "file1.txt", "--output", "file2.txt", "-c"},
    {"hw_03", "-f", "file1.txt", "--output", "file2.txt", "-c"},
    {"hw_03", "-o", "file2.txt", "-c", "-f", "file1.txt"},
    {"hw_03", "-o", "file2.txt", "-c", "--file", "file1.txt"},
    {"hw_03", "--output", "file2.txt", "-c", "--file", "file1.txt"},
    {"hw_03", "--output", "file2.txt", "-c", "-f", "file1.txt"},
    {"hw_03", "-o", "file2.txt", "-f", "file1.txt", "-c"},
    {"hw_03", "-o", "file2.txt", "--file", "file1.txt", "-c"},
    {"hw_03", "--output", "file2.txt", "--file", "file1.txt", "-c"},
    {"hw_03", "--output", "file2.txt", "-f", "file1.txt", "-c"}};  // no, I didn't do that by hand :)

std::vector<std::vector<std::string>> allArgvExtract = {
    {"hw_03", "-u", "-f", "file1.txt", "-o", "file2.txt"},
    {"hw_03", "-u", "--file", "file1.txt", "-o", "file2.txt"},
    {"hw_03", "-u", "--file", "file1.txt", "--output", "file2.txt"},
    {"hw_03", "-u", "-f", "file1.txt", "--output", "file2.txt"},
    {"hw_03", "-u", "-o", "file2.txt", "-f", "file1.txt"},
    {"hw_03", "-u", "-o", "file2.txt", "--file", "file1.txt"},
    {"hw_03", "-u", "--output", "file2.txt", "--file", "file1.txt"},
    {"hw_03", "-u", "--output", "file2.txt", "-f", "file1.txt"},
    {"hw_03", "-f", "file1.txt", "-u", "-o", "file2.txt"},
    {"hw_03", "--file", "file1.txt", "-u", "-o", "file2.txt"},
    {"hw_03", "--file", "file1.txt", "-u", "--output", "file2.txt"},
    {"hw_03", "-f", "file1.txt", "-u", "--output", "file2.txt"},
    {"hw_03", "-f", "file1.txt", "-o", "file2.txt", "-u"},
    {"hw_03", "--file", "file1.txt", "-o", "file2.txt", "-u"},
    {"hw_03", "--file", "file1.txt", "--output", "file2.txt", "-u"},
    {"hw_03", "-f", "file1.txt", "--output", "file2.txt", "-u"},
    {"hw_03", "-o", "file2.txt", "-u", "-f", "file1.txt"},
    {"hw_03", "-o", "file2.txt", "-u", "--file", "file1.txt"},
    {"hw_03", "--output", "file2.txt", "-u", "--file", "file1.txt"},
    {"hw_03", "--output", "file2.txt", "-u", "-f", "file1.txt"},
    {"hw_03", "-o", "file2.txt", "-f", "file1.txt", "-u"},
    {"hw_03", "-o", "file2.txt", "--file", "file1.txt", "-u"},
    {"hw_03", "--output", "file2.txt", "--file", "file1.txt", "-u"},
    {"hw_03", "--output", "file2.txt", "-f", "file1.txt", "-u"}};

std::vector<std::vector<std::string>> incorrectArgvCompress = {
    {},
    {"hw_03"},
    {"hw_03", "-c"},
    {"hw_03", "-c", "-f"},
    {"hw_03", "-c", "-f", "file1.txt"},
    {"hw_03", "-c", "-f", "file1.txt", "-o"},
    {"hw_03", "-c", "-f", "file1.txt", "-o", "file2.txt", "wtf?"},
    {"hw_03", "-c", "-f", "file1.txt", "-o", "file2.txt", "wtf?", "hey, it's too many!"},
    {"hw_03", "-c", "-f", "file1.txt", "-o", "file2.txt", "wtf?", "hey, it's too many!", "somebody help!"},
    {"hw_03", "-c", "-f", "file1.txt", "-o", "file2.txt", "wtf?", "hey, it's too many!", "somebody help!", "aaaaaa"},
    {"hw_03", "-c", "-f", "file1.txt", "-o", "file2.txt", "wtf?", "hey, it's too many!", "somebody help!", "aaaaaa", "........."},
    {"hw_03", "-c", "-f", "file1.txt", "-o", "file2.txt", "wtf?", "hey, it's too many!", "somebody help!", "aaaaaa", ".........", "\0\0\0\0\0\0\0\0"},
    {"hw_03", "-c", "-f", "file1.txt", "-o", "file2.txt", "wtf?", "hey, it's too many!", "somebody help!", "aaaaaa", ".........", "\0\0\0\0\0\0\0\0", "hw_03", "-c", "-f", "file1.txt", "-o", "file2.txt"},
    {"hw_03", "-c", "-f", "file1.txt", "-o", "file2.txt", "wtf?", "hey, it's too many!", "somebody help!", "aaaaaa", ".........", "\0\0\0\0\0\0\0\0", "hw_03", "-c", "-f", "file1.txt", "-o", "file2.txt", "oh shit, here we go again"},
    {"hw_03", "-c", "-f", "-o", "file1.txt", "file2.txt"},
    {"hw_03", "-c", "file1.txt", "-f", "file2.txt", "-o"},
    {"hw_03", "file1.txt", "file2.txt"}
};

std::vector<std::vector<std::string>> incorrectArgvExtract = {
    {},
    {"hw_03"},
    {"hw_03", "-u"},
    {"hw_03", "-u", "-f"},
    {"hw_03", "-u", "-f", "file1.txt"},
    {"hw_03", "-u", "-f", "file1.txt", "-o"},
    {"hw_03", "-u", "-f", "file1.txt", "-o", "file2.txt", "wtf?"},
    {"hw_03", "-u", "-f", "file1.txt", "-o", "file2.txt", "wtf?", "hey, it's too many!"},
    {"hw_03", "-u", "-f", "file1.txt", "-o", "file2.txt", "wtf?", "hey, it's too many!", "somebody help!"},
    {"hw_03", "-u", "-f", "file1.txt", "-o", "file2.txt", "wtf?", "hey, it's too many!", "somebody help!", "aaaaaa"},
    {"hw_03", "-u", "-f", "file1.txt", "-o", "file2.txt", "wtf?", "hey, it's too many!", "somebody help!", "aaaaaa", "........."},
    {"hw_03", "-u", "-f", "file1.txt", "-o", "file2.txt", "wtf?", "hey, it's too many!", "somebody help!", "aaaaaa", ".........", "\0\0\0\0\0\0\0\0"},
    {"hw_03", "-u", "-f", "file1.txt", "-o", "file2.txt", "wtf?", "hey, it's too many!", "somebody help!", "aaaaaa", ".........", "\0\0\0\0\0\0\0\0", "hw_03", "-u", "-f", "file1.txt", "-o", "file2.txt"},
    {"hw_03", "-u", "-f", "file1.txt", "-o", "file2.txt", "wtf?", "hey, it's too many!", "somebody help!", "aaaaaa", ".........", "\0\0\0\0\0\0\0\0", "hw_03", "-u", "-f", "file1.txt", "-o", "file2.txt", "oh shit, here we go again"},
    {"hw_03", "-u", "-f", "-o", "file1.txt", "file2.txt"},
    {"hw_03", "-u", "file1.txt", "-f", "file2.txt", "-o"},
    {"hw_03", "file1.txt", "file2.txt"},
    {"hw_03", "-c","-u","-c","-u","-c"}
};

TEST_CASE("Any argv compress") {
    int argc = 6;
    for (size_t i = 0; i < allArgvCompress.size(); ++i) {
        SUBCASE(std::to_string(i).c_str()) {
            CLI::CLI cli{argc, allArgvCompress[i]};
            CHECK_EQ(cli.getInputFilename(), "file1.txt");
            CHECK_EQ(cli.getOutputFilename(), "file2.txt");
            CHECK_EQ(cli.getMode(), CLI::CLI::Mode::COMPRESS);
        }
    }
}

TEST_CASE("Any argv unpack") {
    int argc = 6;
    for (size_t i = 0; i < allArgvExtract.size(); ++i) {
        SUBCASE(std::to_string(i).c_str()) {
            CLI::CLI cli{argc, allArgvExtract[i]};
            CHECK_EQ(cli.getInputFilename(), "file1.txt");
            CHECK_EQ(cli.getOutputFilename(), "file2.txt");
            CHECK_EQ(cli.getMode(), CLI::CLI::Mode::EXTRACT);
        }
    }
}

TEST_CASE("IncorrectArgv compress") {
    for (size_t i = 0;i < incorrectArgvCompress.size(); ++i) {
        SUBCASE(std::to_string(i).c_str()) {
            int argc = incorrectArgvCompress[i].size();
            CHECK_THROWS_AS(CLI::CLI(argc, incorrectArgvCompress[i]), const CLI::CLIException&);
        }
    }
}

TEST_CASE("IncorrectArgv extract") {
    for (size_t i = 0;i < incorrectArgvExtract.size(); ++i) {
        SUBCASE(std::to_string(i).c_str()) {
            int argc = incorrectArgvExtract[i].size();
            CHECK_THROWS_AS(CLI::CLI(argc, incorrectArgvExtract[i]), const CLI::CLIException&);
        }
    }
}
