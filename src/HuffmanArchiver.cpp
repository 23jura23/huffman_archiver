#include "HuffmanArchiver.hpp"

#include <cstring>
#include <istream>
#include <string>

namespace Huffman {

constexpr size_t BUFFER_MAX_SIZE = 128;

void HuffmanArchiver::initTree(std::istream& is) {
    constexpr size_t BUFFER_SIZE = 100;
    char buffer[BUFFER_SIZE]{};
    while (is) {
        try {
            is.read(buffer, BUFFER_SIZE);
        } catch (const std::ios::failure&) {
            if (!(is.eof() && is.fail())) {
                throw;
            }
            // fail with eof is ok, because the end of file was reached
        }
        for (int i = 0; i < is.gcount(); ++i) {
            tree.incNode(static_cast<unsigned char>(buffer[i]));
        }
    }
    tree.buildTreeForward();
    tree.generateCodes();
    is.clear();
    lastOriginalSize = is.tellg();
    is.seekg(0, std::ios::beg);

    // such order of clear and seekg is important!
    // first clear flags, then seek position.
    // otherwise, eofbit prevent seekg to execute
}

void HuffmanArchiver::initTree(const std::vector<std::pair<char_type, codeType>>& allCodes) {
    for (size_t i = 0; i < allCodes.size(); ++i) {
        tree.setNode(allCodes[i].first, HuffmanNode{allCodes[i].second});
    }
    tree.buildTreeBackward();
}

void HuffmanArchiver::putHeader(std::ostream& os) {
    std::vector<std::pair<char_type, codeType>> allCodes = tree.getAllCodes();

    os.put('H');  // 2 file format bytes
    os.put('F');

    // size of codes table
    os.put(static_cast<uint16_t>(allCodes.size()) >> 8);
    os.put(static_cast<uint16_t>(allCodes.size()) & 0x00FF);

    for (size_t i = 0; i < allCodes.size(); ++i) {
        // character
        os.put(static_cast<uint16_t>(allCodes[i].first) >> 8);
        os.put(static_cast<uint16_t>(allCodes[i].first) & 0x00FF);
        // code length, 2 bytes
        os.put(static_cast<uint16_t>(allCodes[i].second.second) >> 8);
        os.put(static_cast<uint16_t>(allCodes[i].second.second) & 0x00FF);
    }
    // codes array
    for (size_t i = 0; i < allCodes.size(); ++i) {
        pushToBuffer(allCodes[i].second);
    }
    flushBuffer(os, 1);
}

void HuffmanArchiver::getHeader(std::istream& is) {
    char buffer[3]{};
    is.read(buffer, 2);
    if (strcmp(buffer, "HF") != 0)
        throw HuffmanExceptionIO(
            "getHeader: "
            "wrong file format");

    auto get2bytes = [&is]() -> uint16_t {
        uint16_t unsignedBuffer[2]{};
        unsignedBuffer[0] = is.get();
        unsignedBuffer[1] = is.get();
        uint16_t value = (unsignedBuffer[0] << 8) + unsignedBuffer[1];
        return value;
    };

    size_t allCodesSize = get2bytes();

    std::vector<std::pair<char_type, codeType>> allCodes(allCodesSize, {0, {bitStorage{0}, 0}});

    for (size_t i = 0; i < allCodesSize; ++i) {
        allCodes[i].first = get2bytes();
        allCodes[i].second.second = get2bytes();
    }
    uint8_t buffer2{};
    uint8_t pos{};
    for (size_t i = 0; i < allCodes.size(); ++i) {
        for (size_t j = 0; j < allCodes[i].second.second; ++j) {
            if (pos == 0)
                buffer2 = is.get();
            allCodes[i].second.first[j] = (buffer2 >> pos) & 1;
            pos++;
            if (pos == 8) {
                pos = 0;
            }
        }
    }
    initTree(allCodes);
}

void HuffmanArchiver::doCompress(std::istream& is, std::ostream& os) {
    constexpr size_t BUFFER_SIZE = 100;
    char buffer[BUFFER_SIZE]{};
    while (is) {
        try {
            is.read(buffer, BUFFER_SIZE);
        } catch (const std::ios::failure&) {
            if (!(is.eof() && is.fail())) {
                throw;
            }
        }
        for (int i = 0; i < is.gcount(); ++i) {
            const codeType& newCharacter = tree.getCodeByCharacter(static_cast<unsigned char>(buffer[i]));
            pushToBuffer(newCharacter);
        }
        if (BUFFER_MAX_SIZE <= outputBuffer.size())
            flushBuffer(os);
    }
    pushToBuffer(tree.getCodeByCharacter(EOF_CHAR));
    flushBuffer(os, 1);
}

void HuffmanArchiver::doExtract(std::istream& is, std::ostream& os) {
    unsigned char buffer{};
    uint8_t pos{};
    std::optional<char_type> last_char;
    while (true) {
        if (pos == 0)
            buffer = is.get();
        last_char = tree.getCharacterByCode((buffer >> pos) & 1);
        if (last_char.has_value()) {
            if (last_char == EOF_CHAR)
                break;
            os.put(last_char.value());
        }
        pos++;
        if (pos == 8)
            pos = 0;
    }
}

void HuffmanArchiver::pushToBuffer(const codeType& character) {
    if (BYTE_SIZE <= encodedPosition)
        throw HuffmanExceptionInternal("BYTE_SIZE <= encodedPosition = " + std::to_string(encodedPosition));

    for (size_t i = 0; i < character.second; ++i, encodedPosition = (encodedPosition + 1) % BYTE_SIZE) {
        encodedCharacter |= (character.first[i] << encodedPosition);
        if (encodedPosition == BYTE_SIZE - 1) {
            outputBuffer.push_back(encodedCharacter);
            encodedCharacter = 0;
        }
    }
}

void HuffmanArchiver::flushBuffer(std::ostream& os, bool toOffset) {
    if (toOffset && encodedPosition) {
        outputBuffer += encodedCharacter;
        encodedCharacter = 0;
        encodedPosition = 0;
    }
    os << outputBuffer;
    outputBuffer.clear();
}

void HuffmanArchiver::compress(std::istream& is, std::ostream& os) {
    initTree(is);
    putHeader(os);
    lastHeaderSize = os.tellp();
    doCompress(is, os);
    lastProcessedSize = static_cast<size_t>(os.tellp()) - lastHeaderSize.value();
}

void HuffmanArchiver::extract(std::istream& is, std::ostream& os) {
    getHeader(is);
    lastHeaderSize = is.tellg();
    doExtract(is, os);
    lastOriginalSize = static_cast<size_t>(is.tellg()) - lastHeaderSize.value();
    lastProcessedSize = os.tellp();
}

std::tuple<size_t, size_t, size_t> HuffmanArchiver::getLastSizes() {
    if (!lastOriginalSize.has_value() || !lastProcessedSize.has_value() || !lastHeaderSize.has_value())
        throw HuffmanExceptionContract(
            "getLastSize: "
            "No operation were ran yet");
    return std::tie(lastOriginalSize.value(), lastProcessedSize.value(), lastHeaderSize.value());
}

}  // namespace Huffman
