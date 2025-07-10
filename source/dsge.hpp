#include <filesystem>
#include <vector>
#include <sstream>
#include <functional>

#include "applet.hpp"
#include "math.hpp"
#include "random.hpp"
#include "sound.hpp"
#include "sprite.hpp"
#include "text.hpp"
#include "timer.hpp"
#include "touch.hpp"
#include "tween.hpp"
#include "utils.hpp"

struct color {
    u32 transparent;
    u32 white;
    u32 gray;
    u32 black;
    u32 green;
    u32 lime;
    u32 yellow;
    u32 orange;
    u32 red;
    u32 purple;
    u32 blue;
    u32 brown;
    u32 pink;
    u32 magenta;
    u32 cyan;
};

template<typename T>
std::string TSA(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

namespace dsge {
// Forward declarations
namespace _internal {
    // Internal implementation details
    void _logger(const std::string& message);
    void _renderDebugText();
}

// The external color for dsge.
extern color dsgeColor;

// Represents the amount of time in milliseconds that passed since last frame.
extern u64 elapsed;

/**
 * @brief Current background hex color when using the dsge::Render function.
 * 
 * Be careful, if you are debugging and want to set the background color to white, the print output will blend with all of the whiteness! But not to fear, it'll output in the bottom screen instead.
 */
extern u32 bgColor;

/**
 * Initializes dsge and bring back the lives of your own 3DS Games.
 * 
 * Note that if you try to initialize the same function again, there's likely bad things that is gonna happen so don't trigger twice!
 */
void init();

/**
 * Exits all of the needed libraries that DSGE needs
 * 
 * Note that you must initialize this first, there's likely bad things that is gonna happen so don't trigger twice!
 * 
 * It always returns 0 no matter if it's a serious error.
 */
int exit();

/**
 * @brief Checks if both Sprite objects are colliding.
 * @param obj1 The 1st Sprite object to check
 * @param obj2 The 2nd Sprite object to check.
 * @return `true` if both collided, `false` otherwise, or `false` if one of the sprites is invisible.
 * 
 * #### Example Usage:
 * ```
 * dsge::Sprite test1(40, 40);
 * dsge::Sprite test2(40, 40);
 * 
 * dsge::overlap(test1, test2); // will return true since it's both overlapping.
 * ```
 */
bool overlap(dsge::Sprite* obj1, dsge::Sprite* obj2);

void render(std::function<void()> function);

// Public logging macro
#if defined(DEBUG)
    #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__))
    #define print(message) dsge::_internal::_logger(std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + ": " + TSA(message))
#else
    #define print(message) ((void)0)
#endif

// Width of your 3DS
int WIDTH  = 400;

// Height of your 3DS
int HEIGHT = 240;
}

// Implementation details (in header but not exposed in namespace)
namespace dsge::_internal {
    // These are implementation details that shouldn't be used directly
    extern std::vector<Text> _debugText;
    extern std::vector<u8> _debugCol;
    extern C3D_RenderTarget* top;
}