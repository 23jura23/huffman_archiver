#ifndef _INCLUDE_HUFFMAN_HPP
#define _INCLUDE_HUFFMAN_HPP

#include <inttypes.h>

#include <bitset>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Huffman {

// TODO debug ifdefs to hide some too stupid checks

// here and futher: uint32_t is used for character as special characters are needed
typedef uint16_t char_type;

constexpr size_t BYTE_SIZE = 8;

constexpr size_t MAX_LETTER_COUNT = (1 << BYTE_SIZE) + 1;  // + 1 for special EOF character
constexpr size_t MAX_CODE_LENGTH = 2 * MAX_LETTER_COUNT;
// in fact, 2*n-1, but bitset know no difference

constexpr char_type EOF_CHAR = MAX_LETTER_COUNT - 1;

using bitStorage = std::bitset<MAX_CODE_LENGTH>;
using codeType = std::pair<bitStorage, size_t>;

struct HuffmanNode {
    HuffmanNode() = default;
    HuffmanNode(size_t, size_t, size_t);
    explicit HuffmanNode(codeType);
    std::optional<size_t> leftSon, rightSon;
    size_t count = 0;
    /*
     * 1. count of letter in text + 1, if leftSon and rightSon has no value
     * 2. otherwise, summary count of leftSon and rightSon
     *
     * maybe, count = 1?
     * count may be 1 by defauly because of the following case: codes frequencies were analyzed, and the tree of only few characters were built. Then istream (e.g. file) changed and some new characters appeared, that are not in the tree. So what codes do they have?... That's why every character has a starter count so that every tree will be able to return its code.
     * (or just let it go :)
     */

    std::optional<codeType> binaryCode;
};

class HuffmanTree {
 public:
    HuffmanTree();
    void incNode(char_type byte);
    void buildTreeForward();
    void buildTreeBackward();
    void generateCodes();
    void generateCodes(size_t root);

    const codeType& getCodeByCharacter(char_type);
    std::optional<char_type> getCharacterByCode(bool bit);

    std::vector<std::pair<char_type, codeType>> getAllCodes();

    void setNode(size_t pos, const HuffmanNode&);

    const std::vector<HuffmanNode>& getNodes();
    const std::optional<size_t>& getRootIndex();

 private:
    size_t buildTreeBackwardRecursive(size_t depth, bitStorage&, const std::unordered_map<codeType, size_t>& bitMap);
    void generateCodesRecursive(size_t now, size_t depth, bitStorage& currentCode);

    void debugTraverse(size_t now);
    void debugTraverse(size_t now, std::string tab);

    std::vector<HuffmanNode> nodes;
    std::optional<size_t> rootIndex;
    std::optional<size_t> currentNode;
};  // class HuffmanTree


struct HuffmanException : std::runtime_error {
    using std::runtime_error::runtime_error;
    using std::runtime_error::what;
};

struct HuffmanExceptionInternal : HuffmanException {
    using HuffmanException::HuffmanException;
    using HuffmanException::what;
};

struct HuffmanExceptionContract : HuffmanException {
    using HuffmanException::HuffmanException;
    using HuffmanException::what;
};

struct HuffmanExceptionIO : HuffmanException {
    using HuffmanException::HuffmanException;
    using HuffmanException::what;
};

}  // namespace Huffman

namespace std {

template <>
struct hash<Huffman::codeType> {
    size_t operator()(const Huffman::codeType& x) const {
        return std::hash<Huffman::bitStorage>()(x.first) ^ (std::hash<size_t>()(x.second) << 8);
    }
};
}  // namespace std

#endif  // _INCLUDE_HUFFMAN_H
