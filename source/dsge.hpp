#ifndef DSGE_HPP
#define DSGE_HPP

#include <3ds.h>
#include <citro2d.h>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <sstream>

typedef enum {
    BS_BORDER = 0, // Bordered Text, Can be only used on Text Class
    BS_SHADOW = 1  // Shadowed Text, Can be only used on Shadow Class
} bStyle;

typedef enum {
    AXES_X = 0,     // X axis only
    AXES_Y = 1,     // Y axis only
    AXES_XY = 2,    // Both X and Y axes
} axes;

typedef enum {
    ALIGN_LEFT = 0,       // Left alignment for both screens
    ALIGN_CENTER = 1,     // Centered alignment for top screen
    ALIGN_RIGHT = 2,      // Right alignment for top screen
} align;

// Forward declarations for all DSGE components
namespace dsge {
    // Namespaces
    namespace Applet {}
    namespace Math {}
    namespace Random {}
    namespace Utils {}
    namespace Timer {}
    
    // Classes
    class Sound;
    class Sprite;
    class Text;
    class Tween;
    class Touch;
}

// Basic utility headers first
#include "math.hpp"
#include "random.hpp"
#include "utils.hpp"

// Then other headers
#include "applet.hpp"
#include "sound.hpp"
#include "sprite.hpp"
#include "text.hpp"
#include "timer.hpp"
#include "touch.hpp"

// Color structure
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

// Template function for string conversion
template<typename T>
std::string TSA(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

namespace dsge {
// Forward declarations
namespace _internal {
    void _logger(const std::string& message);
    void _renderDebugText();
}

/**
 * @brief The external color for dsge.
 * 
 * You cannot modify this, if you do, something may go wrong with the look of the colors and it may not look right.
 * 
 * #### Example Usage:
 * ```
 * // Getting the green color:
 * dsge::dsgeColor.green // -> 0xff008000;
 * 
 * // Getting the yellow color:
 * dsge::dsgeColor.yellow // -> 0xff00ffff;
 * ```
 */
extern color dsgeColor;

/**
 * @brief Represents the amount of time in milliseconds that passed since last frame.
 * 
 * You cannot modify this, since it always updates the variable every `dsge::Render()`
 * 
 * #### Example Usage:
 * ```
 * // Run the rendering process
 * dsge::Render([&]() {
 *     // Do something
 * });
 * 
 * trace(dsge::elapsed); // Returns the amount of time in millis
 * ```
 */
extern u64 elapsed;

/**
 * @brief Current background hex color when using the dsge::Render function.
 * 
 * Be careful, if you are debugging and want to set the background color to white, the trace output will blend with all of the whiteness! But not to fear, it'll output in the bottom screen instead.
 * 
 * #### Example Usage:
 * ```
 * // Setting bgColor to something else:
 * 
 * dsge::bgColor = dsge::dsgeColor.red; // Sets color to red
 * ```
 */
extern u32 bgColor;

/**
 * @brief Represents the amount of frames per second in total.
 * 
 * Maximum is 60 fps, Counted if dsge::Render is used.
 * 
 * #### Example Usage:
 * ```
 * // Run the rendering process
 * dsge::Render([&]() {
 *     // Do something
 * });
 * 
 * trace(dsge::FPS); // Get the amount of FPS!
 * ```
 */
extern int FPS;

/**
 * @brief Initializes dsge and bring back the lives of your own 3DS Games.
 * 
 * Note that if you try to initialize the same function again, there's likely bad things that is gonna happen so don't trigger twice!
 * 
 * #### Example Usage:
 * ```
 * #include "dsge/dsge.hpp"
 * 
 * int main() {
 *     dsge::init(); // Here is where you init!
 * 
 *     // Rest of the functions below...
 * }
 * ```
 */
void init();

/**
 * @brief Exits all of the needed libraries that DSGE needs
 * 
 * Note that you must initialize this to the very last of `int main`, especially `return dsge::exit()`.
 * 
 * It always returns 0 no matter if it's a serious error.
 * 
 * #### Example Usage:
 * ```
 * #include "dsge/dsge.hpp"
 * 
 * int main() {
 *     // Rest of the functions above...
 * 
 *     return dsge::exit(); // This is where you exit!
 * }
 * ```
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
 * test1.makeGraphic();
 * 
 * dsge::Sprite test2(40, 40);
 * test2.makeGraphic();
 * 
 * dsge::overlap(test1, test2); // will return true since it's both overlapping.
 * ```
 */
bool overlap(Sprite* obj1, Sprite* obj2);

/**
 * @brief Adds a sprite or text to members for dsge::Update;
 * @param basic The sprite or text to add as.
 * 
 * #### Example Usage:
 * ```
 * dsge::Sprite newSprite(120, 120);
 * newSprite.makeGraphic(64, 64);
 * dsge::add(newSprite);
 * 
 * dsge::Text newText(120, 120, "Hello");
 * dsge::add(newText);
 * ```
 */
void add(Sprite& spr);
void add(Text& txt);

/**
 * @brief Starts a function rendering that starts rendering the 3DS's top screen and bottom screen with the concurrent added to members.
 * 
 * This can also render **debug** lines on the top left for clever lookarounds, if you have DEBUG defined, and not used `std::cout` or `printf` and just used `trace` instead.
 * 
 * Always returns aptMainLoop(), always required.
 * 
 * #### Warning:
 * 
 * If the `botScr` actually initializes a function and there are renders in there, it will shift's x axis by 1.25x! Only if it's screen centered or aligned to center/right.
 * 
 * If you use centered or right, please use the `_BOT` enum version!
 * 
 * Render the sprites FIRST then render the text!!! if you don't do that in that order the text will overwrite things making it look ugly.
 * 
 * #### Example Usage:
 * ```
 * // Test Sprite
 * dsge::Sprite newSprite(0, 0);
 * newSprite.makeGraphic(84, 84);
 * 
 * // Test Text
 * dsge::Text newText(0, 0, "New Text!");
 * 
 * // Now render all of it! For top and bottom screen.
 * while (dsge::render()) {
 *     // Code stuff here
 * }
 * ```
 * 
 */
bool render();

// Public logging macro
#if defined(DEBUG)
    #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__))
    #define trace(message) dsge::_internal::_logger(std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + ": " + TSA(message))
#else
    #define trace(message) ((void)0)
#endif

/**
 * @brief The current width of the 3DS.
 * 
 * #### Warning:
 * 
 * NEVER EVER mess with this variable because it's use for screen center and a whole LOT more!! I suggest you to never set anything there.
 * 
 * #### Example Usage:
 * ```
 * trace(dsge::WIDTH); // Returns int 400
 * ```
 */
inline constexpr int WIDTH = 400;

/**
 * @brief The current height of the 3DS.
 * 
 * #### Warning:
 * 
 * NEVER EVER mess with this variable because it's use for screen center and a whole LOT more!! I suggest you to never set anything there.
 * 
 * #### Example Usage:
 * ```
 * trace(dsge::HEIGHT); // Returns int 240
 * ```
 */
inline constexpr int HEIGHT = 240;

/**
 * @brief The current width of the bottom screen for the 3DS.
 * 
 * #### Warning:
 * 
 * NEVER EVER mess with this variable because it's use for screen center and a whole LOT more!! I suggest you to never set anything there.
 * 
 * #### Example Usage:
 * ```
 * trace(dsge::WIDTH_BOTTOM); // Returns int 320
 * ```
 */
inline constexpr int WIDTH_BOTTOM = 320;

// Implementation details (in header but not exposed in namespace)
namespace _internal {
    // These are implementation details that shouldn't be used directly
    extern std::vector<Text> _debugText;
    extern std::vector<u8> _debugCol;
    extern C3D_RenderTarget* top;
    extern std::vector<u64> fpsCtr;
}

} // namespace dsge

#endif