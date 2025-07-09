#include <map>
#include <string>
#include <functional>
#include "sprite.hpp"
#include "text.hpp"

/**
 * @brief Easing functions for tween animations
 * 
 * These functions modify the progress value to create different animation effects.
 * 
 * #### Easing Types:
 * - LINEAR: No easing, linear progression
 * - SINE_IN: Slow start, accelerating
 * - SINE_OUT: Fast start, decelerating
 * - SINE_IN_OUT: Slow start and end, fast middle
 * - QUAD_IN: Quadratic easing in
 * - QUAD_OUT: Quadratic easing out
 * - QUAD_IN_OUT: Quadratic easing in and out
 * - CUBIC_IN: Cubic easing in
 * - CUBIC_OUT: Cubic easing out
 * - CUBIC_IN_OUT: Cubic easing in and out
 * - EXPO_IN: Exponential easing in
 * - EXPO_OUT: Exponential easing out
 * - EXPO_IN_OUT: Exponential easing in and out
 */
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
     * @brief Tweens (interpolates) sprite or text properties over time
     * @param target The sprite or text object to tween
     * @param properties Map of properties to tween with target values { {"x",600}, {"color",0xFF0000FF} }
     * @param duration Duration of tween in seconds
     * @param ease Easing function to use (default: EASE_LINEAR)
     * 
     * #### Details:
     * 
     * Supported properties:
     * - x: horizontal position
     * - y: vertical position
     * - scale.x: horizontal scale
     * - scale.y: vertical scale
     * - color: color value (as float to match other properties)
     * 
     * For color, pass the value as float (e.g., `static_cast<float>(0xFF0000FF)`)
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