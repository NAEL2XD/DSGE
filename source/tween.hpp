#ifndef DSGE_TWEEN_HPP
#define DSGE_TWEEN_HPP

#include <thread>
#include "sprite.hpp"
#include "text.hpp"

typedef enum {
    EASE_LINEAR,     // No easing
    EASE_SINE_IN,    // Slow start
    EASE_SINE_OUT,   // Slow end
    EASE_SINE_IN_OUT,// Slow start and end
    EASE_QUAD_IN,    // Quadratic in
    EASE_QUAD_OUT,   // Quadratic out
    EASE_QUAD_IN_OUT,// Quadratic in-out
    EASE_CUBIC_IN,   // Cubic in
    EASE_CUBIC_OUT,  // Cubic out
    EASE_CUBIC_IN_OUT,// Cubic in-out
    EASE_EXPO_IN,    // Exponential in
    EASE_EXPO_OUT,   // Exponential out
    EASE_EXPO_IN_OUT // Exponential in-out
} eases;

namespace dsge {
class Tween {
public:
    /**
     * @brief Tweens (interpolates) a sprite or text properties over time
     * @param target The sprite or text object to tween
     * @param properties Map of properties to tween with target values `{{"x", 600}, {"color", 0xFF0000FF}}`
     * @param duration Duration of tween in seconds
     * @param ease Easing function to use (default: EASE_LINEAR)
     * 
     * #### Supported Properties:
     * 
     * - `x`: horizontal position.
     * 
     * - `y`: vertical position.
     * 
     * - `scale.x`: horizontal scale.
     * 
     * - `scale.y`: vertical scale.
     * 
     * - `color`: Color value (as float to match other properties).
     * 
     * - `angle`: Angular position.
     * 
     * - `alpha`: Visibility variable.
     * 
     * - `height`: Height variable
     * 
     * - `width`: Width variable
     * 
     * #### Example Usage:
     * ```
     * dsge::Sprite newSprite(0, 0);
     * newSprite.makeGraphic(16, 16);
     * 
     * // Tween x, y and alpha
     * dsge::Tween::tween(newSprite, {{"x", 160}, {"y", 320}, {"alpha", 0.5}});
     * 
     * // Watch it happen
     * while (aptMainLoop()) {
     *     dsge::render([&]() {
     *         newSprite.render();
     *     });
     * }
     * ```
     */
    static void tween(dsge::Sprite& target, const std::map<std::string, float>& properties, float duration, eases ease = EASE_LINEAR);
    static void tween(dsge::Text& target, const std::map<std::string, float>& properties, float duration, eases ease = EASE_LINEAR);

private:
    template<typename T>
    static void tween_impl(T& target, const std::map<std::string, float>& properties, float duration, eases ease);
    
    template<typename T>
    static T lerp(T start, T end, float progress);
    
    static u32 lerpColor(u32 start, u32 end, float progress);
    
    static float applyEase(float progress, eases ease);
};
} // namespace dsge

#endif