#include "dsge.hpp"

namespace dsge {
namespace Touch {
/**
 * @brief Whatever or not you're touching and holding the 3ds's bottom screen
 * @return `true` if is touching and holding bottom screen, `false` otherwise.
 * 
 * #### Example Code:
 * ```
 * dsge::Touch::isTouchHeld(); // Depending on whatever it has been touched.
 * 
 * // Example about game touching:
 * int length = 0;
 * 
 * if (dsge::Touch::isTouchHeld()) {
 *     length++;
 * }
 * ```
 * 
 */
bool isTouchHeld();

/**
 * @brief Whatever or not you've touched the 3ds's bottom screen at the current frame.
 * @return `true` if is touching at the current frame and also in the bottom screen, `false` otherwise.
 * 
 * #### Example Code:
 * ```
 * dsge::Touch::isTouchDown(); // Depending on whatever it has been touched.
 * 
 * // Example about game tapping fast:
 * int taps = 0;
 * 
 * if (dsge::Touch::isTouchDown()) {
 *     taps++;
 * }
 * ```
 * 
 */
bool isTouchDown();

/**
 * @brief Whatever or not you've released touching at the current frame.
 * @return `true` if has been released currently, `false` otherwise.
 * 
 * #### Example Code:
 * ```
 * dsge::Touch::isTouchUp(); // Depending on whatever it has been touched.
 * 
 * // Example about game touching:
 * int ups = 0;
 * 
 * if (dsge::Touch::isTouchUp()) {
 *     ups++;
 * }
 * ```
 * 
 */
bool isTouchUp();

/**
 * @brief Gets the X's position of whatever your 3DS's touching is located.
 * @return Depending on the number of your touch's position (0-320) or -1 if not detected.
 * 
 * #### Example Code:
 * ```
 * print(dsge::Touch::getTouchX());
 * ```
 * 
 */
u16 getTouchX();

/**
 * @brief Gets the Y's position of whatever your 3DS's touching is located.
 * @return Depending on the number of your touch's position (0-240) or -1 if not detected.
 * 
 * #### Example Code:
 * ```
 * print(dsge::Touch::getTouchY());
 * ```
 * 
 */
u16 getTouchY();

/**
 * @brief Returns whetever if the sprite is being touched.
 * @param obj1 The object sprite to check.
 * @returns `true` if touching, `false` otherwise.
 * 
 * #### Example Usage:
 * ```
 * // Create sprite
 * dsge::Sprite obj(0, 0);
 * obj.makeGraphic(120, 120);
 * 
 * // Touching sprite
 * if (dsge::Touch::isTouching(obj)) {
 *     print("Touching!");
 * } else {
 *     print("Not Touching!");
 * }
 * ```
 */
bool isTouching(dsge::Sprite* obj1);
}
}