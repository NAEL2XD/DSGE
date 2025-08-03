#ifndef DSGE_UTILS_HPP
#define DSGE_UTILS_HPP

#include "dsge.hpp"
#include <fstream>

namespace dsge {
namespace Utils {
/**
 * @brief Returns the file read from `sdmc:/` or `romfs:/`.
 * @param filePath The file to read to.
 * @returns The content from the file read.
 * 
 * #### Note:
 * In the `filePath` argument, it must ALWAYS start with `romfs:/` (Aseets, Sounds, etc.) or `sdmc:/` (Saves, Datas, etc.)
 * 
 * #### Example Usage:
 * ```
 * dsge::Utils::readFile("cool_file.txt"); // Will read cool_file.txt from romfs.
 * trace(dsge::Utils::readFile("a_file.txt")); // Converts to a readable trace c string.
 * ```
 */
std::string readFile(const std::string& filePath);

/**
 * @brief Saves a content through sdmc:/.
 * @param filePath The path to save as, do not include `sdmc:/` since it can't be saved in a romfs.
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
 * trace(dsge::Utils::formatBytes(123456789)); // Returns 117MB
 * ```
 */
std::string formatBytes(float byte);

/**
 * @brief Returns the fomatted time from the `ms` arg.
 * 
 * You can add any amount of precision if you want, suggested to be 0 for less CPU work.
 * 
 * @param ms The amount milliseconds to add as.
 * @param precision The very amount of precision to use, more means more decimals.
 * @returns A formatted time string `XX:XX.XX`
 * 
 * #### Example Usage:
 * ```
 * trace(dsge::Utils::formatTime(6000)); // Returns 0:06
 * trace(dsge::Utils::formatTime(1234567, 2)); // Returns 20:34.12
 * ```
 */
std::string formatTime(float ms, int precision = 0);

/**
 * @brief Converts int-dex to a vectored array.
 * @param number The amount of number to use.
 * @returns A vectored array being integers in an all grouped one.
 * 
 * #### Example Usage:
 * ```
 * // Assuming you have an integer
 * int myNum = 3210;
 * 
 * // And an array
 * std::vector<int> arrayNum = {};
 * 
 * // DO this:
 * arrayNum = dsge::Utils::intToArray(myNum);
 * 
 * // trace it all out
 * for (auto &&i : container) {
 *     trace(i); // Prints 3, 2, 1 and 0!
 * }
 * ```
 */
std::vector<int> intToArray(int number);
}
}

#endif