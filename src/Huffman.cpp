#include "Huffman.hpp"

#include <algorithm>
#include <deque>
#include <iostream>
#include <queue>
#include <stack>
#include <string>

namespace Huffman {

HuffmanNode::HuffmanNode(size_t ls, size_t rs, size_t cnt)
        : leftSon{ls}
        , rightSon{rs}
        , count{cnt} {};

HuffmanTree::HuffmanTree()
        : nodes(MAX_LETTER_COUNT) {
    nodes[EOF_CHAR].count++;  // if all codes are zero, no tree will be build
    nodes[0].count++;         // so fictive codes counts are added to guarantee
                              // the tree to be correct
}

HuffmanNode::HuffmanNode(codeType newCode)
        : binaryCode{newCode} {
}

// TODO HufmanTree from some container: vector of frequencies, for example

void HuffmanTree::incNode(char_type byte) {
    if (nodes.size() <= byte)
        throw HuffmanExceptionContract(
            "incNode: "
            "nodes.size() <= byte = " +
            std::to_string(byte));
    nodes[byte].count++;
}

void HuffmanTree::buildTreeForward() {
    if (nodes.size() == 0) {
        throw HuffmanExceptionContract(
            "buildTreeForward: "
            "nodes.size() == 0");
    }
    std::priority_queue<std::pair<size_t, size_t>, std::vector<std::pair<size_t, size_t>>, std::greater<std::pair<size_t, size_t>>> countSortedHeap;
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (nodes[i].count > 0)
            countSortedHeap.emplace(nodes[i].count, i);
    }
    while (countSortedHeap.size() > 1) {
        size_t top1index = countSortedHeap.top().second;
        countSortedHeap.pop();
        size_t top2index = countSortedHeap.top().second;
        countSortedHeap.pop();
        nodes.emplace_back(top1index, top2index, nodes[top1index].count + nodes[top2index].count);
        countSortedHeap.emplace(nodes[nodes.size() - 1].count, nodes.size() - 1);
    }
    rootIndex = nodes.size() - 1;
}

void HuffmanTree::buildTreeBackward() {
    std::unordered_map<codeType, size_t> bitMap;
    for (size_t i = 0; i < nodes.size(); ++i)
        if (nodes[i].binaryCode.has_value()) {
            bitMap[nodes[i].binaryCode.value()] = i;
        }
    bitStorage currentCode{0};
    rootIndex = buildTreeBackwardRecursive(0, currentCode, bitMap);
}

size_t HuffmanTree::buildTreeBackwardRecursive(size_t depth, bitStorage& currentCode, const std::unordered_map<codeType, size_t>& bitMap) {
    if (depth > MAX_CODE_LENGTH)
        throw HuffmanExceptionInternal(
                "buildTreeBackwardRecursive: "
                "Impossible or incomplete codes were providen");
    auto it = bitMap.find(codeType{currentCode, depth});
    if (it != bitMap.end()) {
        return it->second;
    }

    nodes.emplace_back();

    size_t now = nodes.size() - 1;

    currentCode[depth] = 0;
    nodes[now].leftSon = buildTreeBackwardRecursive(depth + 1, currentCode, bitMap);

    currentCode[depth] = 1;
    nodes[now].rightSon = buildTreeBackwardRecursive(depth + 1, currentCode, bitMap);

    currentCode[depth] = 0;
    return now;
}

void HuffmanTree::generateCodes() {
    if (!rootIndex.has_value())
        throw HuffmanExceptionContract(
            "generateCodes: "
            "!rootIndex.has_value()");
    generateCodes(rootIndex.value());
}

void HuffmanTree::generateCodes(size_t root) {
    if (nodes.size() <= root)
        throw HuffmanExceptionContract(
            "generateCodes: "
            "nodes.size() < root = " +
            std::to_string(root));

    // contract checking block
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (nodes[i].leftSon.has_value() != nodes[i].rightSon.has_value()) {
            std::string str_i = std::to_string(i);
            throw HuffmanExceptionContract(
                "generateCodes: "
                "nodes[i = " +
                str_i +
                "].leftSon.has_value() != "
                "nodes[i = " +
                str_i + "].rightSon.has_value()");
        }
        if (nodes[i].leftSon.has_value()) {
            size_t leftSon = nodes[i].leftSon.value();
            size_t rightSon = nodes[i].rightSon.value();
            if (nodes.size() <= leftSon)
                throw HuffmanExceptionInternal(
                    "generateCodes: "
                    "nodes.size() < leftSon = " +
                    std::to_string(leftSon));

            if (nodes.size() <= rightSon)
                throw HuffmanExceptionInternal(
                    "generateCodes: "
                    "nodes.size() < rightSon = " +
                    std::to_string(rightSon));
        }
    }

    bitStorage currentCode;
    generateCodesRecursive(root, 0, currentCode);
}

void HuffmanTree::generateCodesRecursive(size_t now, size_t depth, bitStorage& currentCode) {
    if (!nodes[now].leftSon.has_value()) {
        // only one son may be checked thanks to the contract above
        nodes[now].binaryCode.emplace(currentCode, depth);
        return;
    }

    size_t leftSon = nodes[now].leftSon.value();
    currentCode[depth] = 0;
    generateCodesRecursive(leftSon, depth + 1, currentCode);

    size_t rightSon = nodes[now].rightSon.value();
    currentCode[depth] = 1;
    generateCodesRecursive(rightSon, depth + 1, currentCode);

    currentCode[depth] = 0;
}

//#define TEST_GEN
void HuffmanTree::debugTraverse(size_t now) {
    using std::cerr, std::endl;
#ifdef TEST_GEN
    if (rootIndex.has_value())
        cerr << "CHECK_EQ(root, " << rootIndex.value() << ");\n";
    cerr << "REQUIRE_EQ(cnodes.size(), " << nodes.size() << ");\n";
#endif
    debugTraverse(now, "");
}

void HuffmanTree::debugTraverse(size_t now, std::string tab) {
    using std::cerr, std::endl;

#ifdef TEST_GEN
    cerr << "CHECK_EQ(cnodes[" << now << "].count, " << nodes[now].count << ");\n";
#endif
    if (nodes[now].leftSon.has_value()) {
        debugTraverse(nodes[now].leftSon.value(), tab + "\t");
#ifdef TEST_GEN
        cerr << "CHECK_EQ(cnodes[" << now << "].leftSon.value(), " << nodes[now].leftSon.value() << ");\n";
#endif
    }
    if (now <= 256) {
        std::string s = nodes[now].binaryCode.value().first.to_string();
#ifdef TEST_GEN
        cerr << "CHECK_EQ(cnodes[" << now << "].binaryCode.value().second, " << nodes[now].binaryCode.value().second << ");\n";
        cerr << "CHECK_EQ(cnodes[" << now << "].binaryCode.value().first.to_string(), \"" << nodes[now].binaryCode.value().first.to_string()
             << "\");\n";
#endif
        reverse(s.begin(), s.end());
        s = s.substr(0, nodes[now].binaryCode.value().second);
#ifndef TEST_GEN
        std::cerr << tab << now << ' ' << s << std::endl << std::endl;
#else
#endif
    } else {
#ifndef TEST_GEN
        std::cerr << tab << now << std::endl << std::endl;
#else
        cerr << "CHECK(!cnodes[" << now << "].binaryCode.has_value()"
             << ");\n";
#endif
    }
    if (nodes[now].rightSon.has_value()) {
        debugTraverse(nodes[now].rightSon.value(), tab + "\t");
#ifdef TEST_GEN
        cerr << "CHECK_EQ(cnodes[" << now << "].rightSon.value(), " << nodes[now].rightSon.value() << ");\n";
#endif
    }
}

const codeType& HuffmanTree::getCodeByCharacter(char_type character) {
    if (nodes.size() <= character)
        throw HuffmanExceptionContract(
            "getCodeByCharacter: "
            "nodes.size() <= character = " +
            std::to_string(character));
    if (!nodes[character].binaryCode.has_value()) {
        throw HuffmanExceptionContract(
            "getCode: "
            "nodes[character = " +
            std::to_string(character) +
            "] has no binaryCode value\n"
            "Perhaps, you forgot to generateCodes()?");
    }
    return nodes[character].binaryCode.value();
}

// TODO optimize to get multiple bits at time
std::optional<char_type> HuffmanTree::getCharacterByCode(bool bit) {
    if (!rootIndex.has_value())
        throw HuffmanExceptionContract(
            "getCharacterByCode: "
            "!rootIndex.has_value()"
            "Perhaps, you forgot to generateCodes()?");

    //    if (nodes[rootIndex.value()].binaryCode.has_value()) {
    //        // only 1 character at all
    //        if (!bit)
    //            return rootIndex.value();
    //        else
    //            return std::nullopt;
    //    }
    
    if (!currentNode.has_value())
        currentNode = rootIndex.value();

    if (nodes.size() <= currentNode.value())
        throw HuffmanExceptionInternal(
            "getCharacter: "
            "nodes.size() <= currentNode = " +
            std::to_string(currentNode.value()));

    if (bit) {
        if (!nodes[currentNode.value()].rightSon.has_value())
            throw HuffmanExceptionInternal(
                "getCharacter: "
                "!nodes[currentNode.value() = " +
                std::to_string(currentNode.value()) + "].rightSon.has_value()");

        currentNode.value() = nodes[currentNode.value()].rightSon.value();
    } else {
        if (!nodes[currentNode.value()].leftSon.has_value())
            throw HuffmanExceptionInternal(
                "getCharacter: "
                "!nodes[currentNode.value() = " +
                std::to_string(currentNode.value()) + "].leftSon.has_value()");

        currentNode.value() = nodes[currentNode.value()].leftSon.value();
    }

    if (nodes[currentNode.value()].binaryCode.has_value()) {
        auto retVal = currentNode.value();
        currentNode.value() = rootIndex.value();
        return retVal;
    } else {
        return std::nullopt;
    }
}

std::vector<std::pair<char_type, codeType>> HuffmanTree::getAllCodes() {
    std::vector<std::pair<char_type, codeType>> allCodes;
    for (char_type i = 0; i < nodes.size(); ++i)
        if (nodes[i].count && nodes[i].binaryCode.has_value())
            allCodes.emplace_back(i, nodes[i].binaryCode.value());
    return allCodes;
}

void HuffmanTree::setNode(size_t pos, const HuffmanNode& node) {
    if (nodes.size() <= pos)
        throw HuffmanExceptionContract(
            "setCode: "
            "nodes.siez() <= pos = " +
            std::to_string(pos));
    nodes[pos] = node;
}

const std::vector<HuffmanNode>& HuffmanTree::getNodes() {
    return nodes;
}
const std::optional<size_t>& HuffmanTree::getRootIndex() {
    return rootIndex;
}

}  // namespace Huffman
