#include <fstream>

namespace dsge {
namespace Utils {
/**
 * @brief Returns the file read from `romfs:/`.
 * @param filePath The file to read to, do not include `romfs:/`.
 * @returns The content from the file read.
 * 
 * #### Example Usage:
 * ```
 * dsge::Utils::readFile("cool_file.txt"); // Will read cool_file.txt from romfs.
 * print(dsge::Utils::readFile("a_file.txt")); // Converts to a readable print c string.
 * ```
 */
std::string readFile(const std::string& filePath);

/**
 * @brief Saves a content through sdmc:/.
 * @param filePath The path to save as, do not include `sdmc:/`.
 * @param content The content to save as, can be anything like totally.
 * @returns `true` if file is saved successfully, `false` if the file is badly saved.
 * 
 * #### Example Usage:
 * ```
 * dsge::Utils::saveFile("file.txt", "Hello from DSGE!"); // Saves a file to DSGE!
 * 
 * const char* thing = "File from a char!";
 * std::string converted_thing = std::to_string(thing);
 * dsge::Utils::saveFile("file2.txt", converted_thing); // Also works with other types of variables if it's converted to string.
 * ```
 */
bool saveFile(const std::string& filePath, const std::string& content);

/**
 * @brief Returns a formatted byte sized string.
 * @param byte The float amount you want to set as.
 * @returns A formatted string exampling from `XBytes`, `XkB`, `XmB`, etc.
 * 
 * #### Example Usage:
 * ```
 * print(dsge::Utils::formatBytes(123456789)); // Returns 117MB
 * ```
 */
std::string formatBytes(float byte);
}
}