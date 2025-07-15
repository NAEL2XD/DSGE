#include <citro2d.h>
#include <string>
#include <filesystem>
#include "sprite.hpp"

typedef enum {
    BS_Border = 0,
    BS_Shadow = 1
} bStyle;

typedef enum {
    ALIGN_LEFT = 0,          // Left alignment for both top and bottom screen.
    ALIGN_CENTER = 1,       // Centered alignment for the top screen.
    ALIGN_RIGHT = 2,       // Right alignment for the top screen.
    ALIGN_CENTER_BOT = 3, // Centered alignment for the bottom screen.
    ALIGN_RIGHT_BOT = 4, // Right alignment for the bottom screen.
} align;

namespace dsge {
class Text {
public:
    align       alignment;   // The alignment to set for the text.
    float       alpha;       // Alpha transparency (0 = invisible, 1 = fully visible)
    float       angle;       // Angular rotation in degrees.
    bStyle      borderStyle; // Style of the border, can be `BS_Border` or `BS_Shadow`.
    bool        bold;        // Bold flag
    u32         borderColor; // Border color
    float       borderSize;  // Border size
    u32         color;       // Text color
    bool        flipX;       // Horizontal flip.
    bool        flipY;       // Vertical flip.
    C2D_Font    font;        // Font to use
    float       height;      // Height of the text.
    std::string text;        // Text content
    bool        underline;   // Underlined text
    bool        visible;     // Visibility flag
    float       width;       // Width of the text.
    int         x;           // X position
    int         y;           // Y position

    struct {
        float x; // X's Acceleration speed.
        float y; // Y's Acceleration speed.
    } acceleration;

    struct {
        float x;        // Horizontal scale
        float y;        // Vertical scale

        /**
         * @brief Sets both x and y scale simultaneously.
         * @param x Horizontal scale. 1 by default.
         * @param y Vertical scale. 1 by default.
         * 
         * #### Example Usage:
         * ```
         * dsge::Text text(50, 50);
         * text.scale.set(1.5, 1.5); // Scales to 1.5
         * text.scale.set(); // Will scale to 1 by default.
         * ```
         */
        void set(float x = 1, float y = 1) {
            this->x = x;
            this->y = y;
        }
    } scale;

    struct {
        C3D_Mtx matrix;
        bool debug;
        bool destroyed;
    } _private;
    

    /**
     * @brief Constructor: Creates a new Text object
     * @param x X position
     * @param y Y position
     * @param Text Text content
     * 
     * #### Example Usage:
     * ```
     * dsge::Text text(60, 30, "Hello, DSGE!"); // Makes a text on pos x 60 y 30 with text saying "Hello, DSGE!".
     * dsge::Text text(); // Makes an invisible text.
     * ```
     */
    Text(int x = 0, int y = 0, const std::string& Text = "");

    /**
     * @brief Centers the text on the screen.
     * @param pos Type of axes position to use, can be `AXES_X`, `AXES_Y`, `AXES_XY`.
     * 
     * #### Example Usage:
     * ```
     * dsge::Text text();
     * text.screenCenter(); // Will center the text by screen.
     * 
     * // Or notably
     * text.screenCenter(AXES_X); // Centers the x itself.
     * ```
     */
    void screenCenter(axes pos = AXES_XY);

    /**
     * @brief Checks if the text is on screen or not.
     * @returns `true` if it's on screen, `false` otherwise.
     * 
     * #### Example Code:
     * ```
     * dsge::Text text(-40, 50);
     * text.isOnScreen(); // Will return false since it's not on screen.
     * ```
     */
    bool isOnScreen();

    /**
     * @brief Loads a new font in `romfs:/` without the prefix.
     * @param filePath The file path to load as (e.g. vcr.bcfnt)
     * @returns `true` if loaded successfully, `false` otherwise.
     * 
     * #### Example Usage:
     * ```
     * // Assuming you have a font.
     * dsge::Text newText(0, 0, "New Font!");
     * if (newText.loadFont("vcr.bcfnt")) {
     *     print("Success!");
     * } else {
     *     print("Failure...");
     * }
     * 
     * // Assuming you want to use the note from a variable, this does not need to be used:
     * C2D_Font fnt = C2D_FontLoad("romfs:/vcr.bcfnt");
     * newText.font = fnt;
     * ```
     */
    bool loadFont(std::string filePath);

    /**
     * @brief Renders the Text (must be in a rendering frame)
     * 
     * #### Example Usage:
     * ```
     * dsge::Text text(60, 30, "Hello, DSGE!");
     * text.render(); // Renders a text.
     * ```
     */
    void render();

    /**
     * @brief Frees text's specific variables and cleans up memory so you can handle making more rendered texts.
     * 
     * For Null Safety reasons, there's a specific variable that's not gonna be freed but will stop you from rendering, or doing text's stuff.
     */
    void destroy();

    static void init();
    static void exit();

private:
    static C2D_Font defaultFont;    // Default system font
    static C2D_TextBuf g_staticBuf; // Text buffer

    void createText();

    // Helper to apply alpha to a color
    static u32 applyAlpha(u32 color, float alpha);
};

} // namespace dsge