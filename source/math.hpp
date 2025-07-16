#ifndef DSGE_MATH_HPP
#define DSGE_MATH_HPP

#include "sprite.hpp"
#include <math.h>

namespace dsge {
namespace Math {
/**
 * Find the distance (in pixels, rounded) between two Sprite, taking their origin into account.
 *
 * @param SpriteA The first Sprite.
 * @param SpriteB The second Sprite.
 * @return Distance between the sprites in pixels.
 * 
 * #### Example Usage:
 * ```
 * dsge::Sprite test1(40, 40);
 * dsge::Sprite test2(80, 80);
 * 
 * dsge::Math::distanceBetween(test1, test2); // Will return 56.57.
 * ```
 */
double distanceBetween(dsge::Sprite spriteA, dsge::Sprite spriteB);

/**
 * @brief Linearly interpolates between two values.
 * 
 * Performs a linear interpolation between values `a` and `b` using parameter `t`.
 * 
 * When `t` is 0, returns `a`. When `t` is 1, returns `b`. Values of `t` outside [0, 1]
 * 
 * will extrapolate beyond the [a, b] range.
 * 
 * @param a The start value (returned when t = 0)
 * @param b The end value (returned when t = 1)
 * @param t The interpolation parameter (typically between 0 and 1)
 * @return The interpolated value between a and b
 * 
 * #### Example Usage:
 * ```
 * dsge::Math::lerp(0, 10, 0);   // Returns 0.0
 * dsge::Math::lerp(0, 10, 0.5);   // Returns 5.0
 * dsge::Math::lerp(0, 10, 1);   // Returns 10.0
 * dsge::Math::lerp(10, 20, 0.25); // Returns 12.5
 * dsge::Math::lerp(5, -5, 0.5);   // Returns 0.0
 * ```
 */
float lerp(float a, float b, float t);

/**
 * @brief Converts angle to compatible radians, this is used in sprite and text for angle support.
 * @param angle The angle set to be converted as.
 * @return A number with the converted radian
 * 
 * #### Example Usage:
 * ```
 * dsge::Math::angleToRadians(20); // Returns 0.3490658504 or 0.1111111111π
 * dsge::Math::angleToRadians(90); // Returns 1.5707963268 or 0.5π
 * ```
 */
double angleToRadians(double angle);
}
}

#endif