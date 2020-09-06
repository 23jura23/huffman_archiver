#ifndef _INCLUDE_HUFFMAN_ARCHIVER_HPP
#define _INCLUDE_HUFFMAN_ARCHIVER_HPP

#include <string>
#include <utility>

#include "Huffman.hpp"

namespace Huffman {

class HuffmanArchiver {
 public:
    void compress(std::istream&, std::ostream&);
    void extract(std::istream&, std::ostream&);
    std::tuple<size_t, size_t, size_t> getLastSizes();

 private:
    void initTree(std::istream&);  // after reading the file sets istream file pointer to the beginning
    void initTree(const std::vector<std::pair<char_type, codeType>>&);
    void putHeader(std::ostream&);
    void doCompress(std::istream&, std::ostream&);  // actually compress characters
    void pushToBuffer(const codeType&);
    void flushBuffer(std::ostream& os, bool toOffset = 0);

    void getHeader(std::istream&);
    void doExtract(std::istream&, std::ostream&);  // actually decompress characters

 private:
    HuffmanTree tree;
    char_type encodedCharacter{};
    char_type encodedPosition{};
    std::string outputBuffer;

    std::optional<size_t> lastOriginalSize, lastProcessedSize, lastHeaderSize;
};  // class HuffmanArchiver

}  // namespace Huffman

#endif  // _INCLUDE_HUFFMAN_ARCHIVER_HPP
