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
 * print(dsge::elapsed); // Returns the amount of time in millis
 * ```
 */
extern u64 elapsed;

/**
 * @brief Current background hex color when using the dsge::Render function.
 * 
 * Be careful, if you are debugging and want to set the background color to white, the print output will blend with all of the whiteness! But not to fear, it'll output in the bottom screen instead.
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
 * Maximum is 60 fps, counted if dsge::Render is used.
 * 
 * #### Example Usage:
 * ```
 * // Run the rendering process
 * dsge::Render([&]() {
 *     // Do something
 * });
 * 
 * print(dsge::FPS); // Get the amount of FPS!
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
 * #include <dsge.hpp>
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
 * #include <dsge.hpp>
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
 * dsge::Sprite test2(40, 40);
 * 
 * dsge::overlap(test1, test2); // will return true since it's both overlapping.
 * ```
 */
bool overlap(dsge::Sprite* obj1, dsge::Sprite* obj2);

/**
 * @brief Starts a function rendering that starts rendering the 3DS's top screen with what sprite renders you want to use.
 * 
 * This can also render **debug** lines on the top left for clever lookarounds.
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
 * while (aptMainLoop()) {
 *     // Now render all of it!
 *     dsge::render([&]() {
 *         newSprite.render();
 *         newText.render();
 *     });
 * }
 * ```
 */
void render(std::function<void()> function);

// Public logging macro
#if defined(DEBUG)
    #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__))
    #define print(message) dsge::_internal::_logger(std::string(__FILENAME__) + ":" + std::to_string(__LINE__) + ": " + TSA(message))
#else
    #define print(message) ((void)0)
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
 * print(dsge::WIDTH); // Returns int 400
 * ```
 */
int WIDTH  = 400;

/**
 * @brief The current height of the 3DS.
 * 
 * #### Warning:
 * 
 * NEVER EVER mess with this variable because it's use for screen center and a whole LOT more!! I suggest you to never set anything there.
 * 
 * #### Example Usage:
 * ```
 * print(dsge::height); // Returns int 400
 * ```
 */
int HEIGHT = 240;
}

// Implementation details (in header but not exposed in namespace)
namespace dsge::_internal {
    // These are implementation details that shouldn't be used directly
    extern std::vector<Text> _debugText;
    extern std::vector<u8> _debugCol;
    extern C3D_RenderTarget* top;
    extern std::vector<u64> fpsCtr;
}