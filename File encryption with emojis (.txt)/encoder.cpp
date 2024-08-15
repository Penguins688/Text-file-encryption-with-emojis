#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <sstream>

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

int main() {
    std::string filepath;
    std::cout << "Enter the file path: " << std::endl;
    std::cin >> filepath;

    std::ifstream inFile(filepath);
    if (!inFile) {
        std::cerr << "Error: could not open file." << std::endl;
        return -1;
    }

    std::stringstream content;
    content << inFile.rdbuf();

    std::string output;
    for (char character : content.str()) {
        for (int i = 0; i < 95; i++) {
            if (character == asciiChars[i]) {
                output += emojis[i];
                break;
            }
        }
        if (character == '\n') {
            output += '\n';
        }
    }

    std::ofstream outFile(removeExtension(filepath) + ".emoji");
    outFile << output;

    return 0;
}
