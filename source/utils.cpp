#include "dsge.hpp"
#include "utils.hpp"

namespace dsge {
namespace Utils {
std::string readFile(const std::string& filePath) {
    std::ifstream file("romfs:/" + filePath); // Open the file
    if (!file.is_open()) {
        print("Utils::readFile: Could not open file: romfs:/" + filePath);
        return "";
    }

    std::string content, line;
    while (std::getline(file, line)) { // Read line by line
        content += line + '\n';
    }

    file.close(); // Close the file
    return content;
}

bool saveFile(const std::string& filePath, const std::string& content) {
    std::ofstream file("sdmc:/" + filePath);
    if (!file.is_open()) {
        print("Utils::saveFile: Could not open file: sdmc:/" + filePath);
        return false;
    }

    file << content;
    file.close();
    return file.good();
}
}
}
