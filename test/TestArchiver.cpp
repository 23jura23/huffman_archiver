#include <doctest.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include "HuffmanArchiver.hpp"

using namespace Huffman;

/*
 useful command
$ find test/samples -type f -name "*" | grep -v -E ".*\.hf" | xargs -n 1 -I % ./bin/hw_03 -c -f "%" -o "%.hf"
 */

std::vector<std::string> testFiles = {"test/samples/TestEmpty",
                                      "test/samples/TestQwerty",
                                      "test/samples/TestRandomShit1",
                                      "test/samples/TestFibonacci"};

TEST_CASE("HuffmanArchiver.compress(istream&)") {
    HuffmanArchiver ht;
    std::string testFile, ansFile;
    for (auto i : testFiles) {
        SUBCASE(i.c_str()) {
            testFile = i;
            ansFile = i + ".hf";
        }
    }

    auto testStream = std::ifstream(testFile, std::ios::in | std::ios::binary);
    auto ansStream = std::ifstream(ansFile, std::ios::in | std::ios::binary);
    std::stringstream sso;

    std::string ans;
    char buf{};
    while (ansStream.get(buf)) {
        ans += buf;
    }

    ht.compress(testStream, sso);

    CHECK_EQ(sso.str(), ans);

    testStream.close();
    ansStream.close();
}

TEST_CASE("HuffmanArchiver.extract(istream&)") {
    HuffmanArchiver ht;
    std::string testFile, ansFile;
    for (auto i : testFiles) {
        SUBCASE(i.c_str()) {
            testFile = i + ".hf";
            ansFile = i;
        }
    }

    auto testStream = std::ifstream(testFile, std::ios::in | std::ios::binary);
    auto ansStream = std::ifstream(ansFile, std::ios::in | std::ios::binary);
    std::stringstream sso;

    std::string ans;
    char buf{};
    while (ansStream.get(buf)) {
        ans += buf;
    }

    ht.extract(testStream, sso);

    CHECK_EQ(sso.str(), ans);

    testStream.close();
    ansStream.close();
}

TEST_CASE("HuffmanArchiver full cycle") {
    HuffmanArchiver ht;
    std::string testFile, ansFile;
    for (auto i : testFiles) {
        SUBCASE(i.c_str()) {
            testFile = i;
            ansFile = i;
        }
    }

    auto testStream = std::ifstream(testFile, std::ios::in | std::ios::binary);
    std::stringstream sso, ssres;

    ht.compress(testStream, sso);
    ht.extract(sso, ssres);

    testStream.close();

    auto ansStream = std::ifstream(ansFile, std::ios::in | std::ios::binary);
    std::string ans;
    char buf{};
    while (ansStream.get(buf)) {
        ans += buf;
    }

    CHECK_EQ(ssres.str(), ans);

    ansStream.close();
}
