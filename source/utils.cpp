#include "utils.hpp"

namespace dsge {
namespace Utils {
std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath); // Open the file
    if (!file.is_open()) {
        trace("Utils::readFile: Could not open file: " + filePath);
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
        trace("Utils::saveFile: Could not save file: sdmc:/" + filePath);
        return false;
    }

    file << content;
    file.close();
    return file.good();
}

std::string formatBytes(float byte) {
    std::string units[6] = {"Bytes", "kB", "MB", "GB", "TB", "PB"};
    int unit = 0;

    while (byte >= 1024 && unit < 6) {
        byte /= 1024;
        unit++;
    }

    return std::to_string((int)byte) + units[unit];
}

std::string formatTime(float ms, int precision) {
    int total = static_cast<int>(ms / 1000);
    
    int seconds = total % 60;
    int minutes = (total / 60) % 60;
    int hours = total / 3600;

    std::string secs = (seconds < 10) ? "0" + std::to_string(seconds) : std::to_string(seconds);
    std::string mins = (hours > 0) ? "0" + std::to_string(minutes) : std::to_string(minutes);
    std::string out = mins + ":" + secs;

    if (hours > 0) {
        out = std::to_string(hours) + ":" + out;
    }
    
    if (precision > 0) {
        std::string precision_str = std::to_string(static_cast<int>(total * std::pow(10, precision)));
        if (precision_str.length() < static_cast<size_t>(precision)) {
            precision_str = std::string(precision - precision_str.length(), '0') + precision_str;
        }
        
        precision_str = precision_str.replace(precision, precision_str.length(), "");
        out += "." + precision_str;
    }

    return out;
}

std::vector<int> intToArray(int number) {
    std::vector<int> digits;

    if (number == 0) digits.push_back(0);
    while (number > 0) {
        digits.insert(digits.begin(), number % 10);
        number /= 10;
    }

    return digits;
}
}
}