#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

const std::string emojis[95] = {
    "😀", "😁", "😂", "🤣", "😃", "😄", "😅", "😆", "😇", "😈",
    "😉", "😊", "😎", "😍", "😘", "😗", "😙", "😚", "😋", "😜",
    "😝", "😛", "🤑", "🤗", "🤔", "🤐", "🤒", "🤓", "🤥", "🤕",
    "🤯", "😳", "🥺", "😢", "😭", "😤", "😠", "😡", "😶", "😐",
    "😑", "😒", "🙄", "🤤", "😴", "😵", "😲", "🥵", "🥶", "😱",
    "😨", "😰", "😥", "😈", "👿", "💀", "☠️", "👻", "👽", "💩",
    "🤖", "👾", "🎃", "🎅", "🎄", "🎆", "🎇", "🎈", "🎉", "🎊",
    "🎁", "🎏", "🎐", "💘", "💝", "💖", "💗", "💓", "💞", "💕",
    "💌", "💟", "☮️", "🕉️", "✡️", "🔯", "☸️", "☯️", "🔥", "😶", 
    "🌿", "🕊️", "🦤", "🍀", "🦚"
};

const char asciiChars[95] = {
    ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
    '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_',
    '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
    'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~'
};

std::string removeExtension(const std::string& filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos) {
        return filename;
    }
    return filename.substr(0, dotPos);
}

std::vector<uint32_t> utf8_to_codepoints(const std::string& utf8_string) {
    std::vector<uint32_t> codepoints;
    for (size_t i = 0; i < utf8_string.length();) {
        uint32_t codepoint = 0;
        if ((utf8_string[i] & 0b10000000) == 0) {
            codepoint = utf8_string[i];
            i += 1;
        } else if ((utf8_string[i] & 0b11100000) == 0b11000000) {
            codepoint = ((utf8_string[i] & 0b00011111) << 6) | (utf8_string[i+1] & 0b00111111);
            i += 2;
        } else if ((utf8_string[i] & 0b11110000) == 0b11100000) {
            codepoint = ((utf8_string[i] & 0b00001111) << 12) | ((utf8_string[i+1] & 0b00111111) << 6) | (utf8_string[i+2] & 0b00111111);
            i += 3;
        } else if ((utf8_string[i] & 0b11111000) == 0b11110000) {
            codepoint = ((utf8_string[i] & 0b00000111) << 18) | ((utf8_string[i+1] & 0b00111111) << 12) | ((utf8_string[i+2] & 0b00111111) << 6) | (utf8_string[i+3] & 0b00111111);
            i += 4;
        }
        codepoints.push_back(codepoint);
    }
    return codepoints;
}

int main() {
    std::string filepath;
    std::cout << "Enter the file path: " << std::endl;
    std::getline(std::cin, filepath);

    std::ifstream inFile(filepath, std::ios::binary);
    if (!inFile) {
        std::cerr << "Error: could not open file." << std::endl;
        return -1;
    }

    std::string input((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());

    std::unordered_map<uint32_t, char> emojiToAscii;
    for (int i = 0; i < 95; i++) {
        std::vector<uint32_t> codepoints = utf8_to_codepoints(emojis[i]);
        if (!codepoints.empty()) {
            emojiToAscii[codepoints[0]] = asciiChars[i];
        }
    }

    std::string output;
    std::vector<uint32_t> inputCodepoints = utf8_to_codepoints(input);

    for (uint32_t codepoint : inputCodepoints) {
        auto it = emojiToAscii.find(codepoint);
        if (it != emojiToAscii.end()) {
            output += it->second;
        } else {
            // If not found in our mapping, keep the original character
            char utf8_char[5] = {0};
            if (codepoint <= 0x7F) {
                utf8_char[0] = codepoint;
            } else if (codepoint <= 0x7FF) {
                utf8_char[0] = 0xC0 | (codepoint >> 6);
                utf8_char[1] = 0x80 | (codepoint & 0x3F);
            } else if (codepoint <= 0xFFFF) {
                utf8_char[0] = 0xE0 | (codepoint >> 12);
                utf8_char[1] = 0x80 | ((codepoint >> 6) & 0x3F);
                utf8_char[2] = 0x80 | (codepoint & 0x3F);
            } else {
                utf8_char[0] = 0xF0 | (codepoint >> 18);
                utf8_char[1] = 0x80 | ((codepoint >> 12) & 0x3F);
                utf8_char[2] = 0x80 | ((codepoint >> 6) & 0x3F);
                utf8_char[3] = 0x80 | (codepoint & 0x3F);
            }
            output += utf8_char;
        }
    }

    std::ofstream outFile(removeExtension(filepath) + ".decoded");
    outFile << output;

    std::cout << "Decoding complete. Output saved to " << removeExtension(filepath) + ".decoded" << std::endl;

    return 0;
}