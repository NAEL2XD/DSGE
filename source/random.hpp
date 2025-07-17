#ifndef DSGE_RANDOM_HPP
#define DSGE_RANDOM_HPP

#include "dsge.hpp"

namespace dsge {
namespace Random {
/**
 * @brief Returns a pseudorandom integer between min and max.
 * @param min The minimum value that should be returned. 1 by default.
 * @param max The maximum value that should be returned. 2,147,483,647 by default.
 * @return A Random number depending on the arguments.
 * 
 * #### Example Usage:
 * ```
 * dsge::Random::integer(1, 15); // Will return a number between 1 to 15
 * ```
 */
int integer(int min = 1, int max = 2147483647);

/**
 * @brief Returns a pseudorandom float value between Min (inclusive) and Max (exclusive).
 * @param min The minimum value that should be returned. 1 by default.
 * @param max The maximum value that should be returned. 2,147,483,647 by default.
 * @param decimal How many digits to include?
 * @return A Random number depending on the arguments.
 * 
 * #### Example Usage:
 * ```
 * dsge::Random::floating(1, 15, 5); // Will return a number between 1.00000 to 15.00000
 * dsge::Random::floating(20, 40, 1) // WIll return a number between 20.00 to 40.00
 * ```
 */
float floating(float min = 1, float max = 2147483647, int decimal = 2);

/**
 * @brief Returns true or false based on the chance value (default 50%).
 * 
 * For example if you wanted a player to have a 30.5% chance of getting a bonus, call bool(30.5) - true means the chance passed, false means it failed.
 * @param chance The chance of receiving the value. Should be given as a number between 0 and 100 (effectively 0% to 100%). 50 by default.
 * @return Whether the roll passed or not.
 * 
 * #### Example Usage:
 * ```
 * dsge::Random::boolean(50); // It will return a 50% chance of being true or false.
 * dsge::Random::boolean(30.5); // It will return a 30.5% chance of being true or false.
 * ```
 */
bool boolean(float chance = 50);

/**
 * @brief Returns a random color by ranging through 0xFF000000 (black) to 0xFFFFFFFF (white).
 * 
 * #### Example Usage:
 * ```
 * dsge::Random::color(); // Returns a random color ranging through 0xFF000000 to 0xFFFFFFFF
 * ```
 */
u32 color();
}
}

#endif