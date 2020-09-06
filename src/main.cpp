#include <fstream>
#include <iostream>

#include "CLI.hpp"
#include "HuffmanArchiver.hpp"

using Huffman::HuffmanArchiver;
using std::cerr, std::cout, std::endl;

const std::string intro = "Huffman archiver\n"
    "Usage: huffman (-c | -u) (-f | --file) input (-o | --output) output\n"
    "\n"
    "-c             for archiving\n"
    "-u             for extracting\n"
    "-f or --file   file to archive/extract\n"
    "-o or --output file to write result\n";


int main(int argc, char** argv) {
    if (argc == 1) {
        cout << intro << endl;
        return 0;
    }

    try {
        std::vector<std::string> stringArgv;
        for (int i = 0; i < argc; ++i) stringArgv.push_back(argv[i]);
        CLI::CLI cli(argc, stringArgv);
        auto ifs = std::ifstream{cli.getInputFilename(), std::ios::in | std::ios::binary};
        ifs.exceptions(std::istream::failbit | std::istream::badbit);
        auto ofs = std::ofstream{cli.getOutputFilename(), std::ios::out | std::ios::binary};
        ofs.exceptions(std::istream::failbit | std::istream::badbit);
        HuffmanArchiver archiver;
        switch (cli.getMode()) {
            case CLI::CLI::Mode::COMPRESS:
                archiver.compress(ifs, ofs);
                break;
            case CLI::CLI::Mode::EXTRACT:
                archiver.extract(ifs, ofs);
                break;
        }
        auto [original, processed, header] = archiver.getLastSizes();
        cout << "Original size:   " << original << endl;
        cout << "Compressed size: " << processed << endl;
        cout << "Header size:     " << header << endl;
        ofs.close();
        ifs.close();
    } catch (const CLI::CLIException& e) {
        cerr << e.what() << endl;
        return 1;
    } catch (const std::ios::failure& e) {
        cerr << e.what() << endl;
        return 2;
    } catch (const Huffman::HuffmanExceptionContract& e) {
        cerr << "HuffmanExceptionContract: " << e.what() << endl;
        return 3;
    } catch (const Huffman::HuffmanExceptionIO& e) {
        cerr << "HuffmanExceptionIO: " << e.what() << endl;
        return 4;
    } catch (const Huffman::HuffmanExceptionInternal& e) {
        cerr << "HuffmanExceptionInternal: " << e.what() << endl;
        return 253;
    } catch (const Huffman::HuffmanException& e) {
        cerr << "HuffmanException: " << e.what() << endl;
        return 254;
    } catch (const std::exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return 5;
    } catch (...) {
        cerr << "Unexpected error" << endl;
        return 255;
    }
}
