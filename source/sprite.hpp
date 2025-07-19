#ifndef DSGE_SPRITE_HPP
#define DSGE_SPRITE_HPP

#include "dsge.hpp"

namespace dsge {
class Sprite {
public:
    float alpha;    // Alpha transparency (0 = invisible, 1 = fully visible)
    float angle;    // Rotation angle.
    u32   color;    // Sprite color, only works if it isn't an image!
    bool  flipX;    // Horizontal flip.
    bool  flipY;    // Vertical flip.
    u16   height;   // Sprite height.
    bool  visible;  // Sprite visibility.
    u16   width;    // Sprite width.
    float x;        // X Position of Sprite.
    float y;        // Y Position of Sprite.

    struct {
        C2D_Image image;
        C2D_SpriteSheet sprite;
        bool destroyed;
        C2D_ImageTint tint;
        C3D_Mtx matrix;
    } _private;

    struct {
        float x; // Horizontal scale.
        float y; // Vertical scale.

        /**
         * @brief Sets both x and y scale simultaneously.
         * @param x Horizontal scale. 1 by default.
         * @param y Vertical scale. 1 by default.
         * 
         * #### Example Usage:
         * ```
         * dsge::Sprite sprite(50, 50);
         * sprite.scale.set(1.5, 1.5); // Scales to 1.5
         * sprite.scale.set(); // Will scale to 1 by default.
         * ```
         */
        void set(float x = 1, float y = 1) {
            this->x = x;
            this->y = y;
        }
    } scale;

    struct {
        float x; // X's Acceleration speed.
        float y; // Y's Acceleration speed.
        float angle; // Angle's Acceleration speed.
    } acceleration;

    /**
     * @brief Constructor: Creates a new Sprite at position (x, y).
     * @param x The X position of your sprite.
     * @param y The Y position of your sprite.
     * @return The whole sprite's data.
     * 
     * #### Example Usage:
     * ```
     * dsge::Sprite sprite(50, 50); // Makes a sprite in position x 50 y 50.
     * dsge::Sprite sprite(); // Makes a sprite in position x 0 y 0
     * ```
     */
    Sprite(int x = 0, int y = 0);

    /**
     * @brief Creates a rectangular graphic for the Sprite.
     * @param width  Width of the graphic.
     * @param height Height of the graphic.
     * @param color  Color of the graphic.
     * @return Sprite's graphic being made.
     * 
     * #### Example Usage:
     * ```
     * dsge::Sprite sprite(50, 50);
     * sprite.makeGraphic(60, 30, 0xFFFF0000); // Makes graphic sprite.
     * sprite.makeGraphic(); // Makes graphic sprite without having to do some options.
     * ```
     */
    void makeGraphic(int width = 128, int height = 64, u32 color = 0xFFFFFFFF);

    /**
     * @brief Loads a .t3x image from romfs.
     * @param file Path to image file (without "romfs:/" prefix).
     * @returns `true` if successful, `false` otherwise.
     * 
     * #### Example Usage:
     * ```
     * dsge::Sprite sprite(50, 50);
     * sprite.loadGraphic("dsge.t3x"); // Loads an image in romfs.
     * ```
     */
    bool loadGraphic(const std::string& file);

    /**
     * @brief Checks if the sprite is on screen or not.
     * @returns `true` if it's on screen, `false` otherwise.
     * 
     * #### Example Code:
     * ```
     * dsge::Sprite sprite(-40, 50);
     * sprite.makeGraphic(10);
     * sprite.isOnScreen(); // Will return false since it's not on screen.
     * ```
     */
    bool isOnScreen();

    /**
     * @brief Renders the Sprite (must be in a rendering frame)
     * 
     * #### Example Usage:
     * ```
     * dsge::Sprite sprite();
     * sprite.render(); // Renders a sprite.
     * ```
     */
    void render();

    /**
     * @brief Centers the Sprite on the screen.
     * @param pos Type of axes position to use, can be `AXES_X`, `AXES_Y`, `AXES_XY`.
     * 
     * #### Example Usage:
     * ```
     * dsge::Sprite sprite();
     * sprite.screenCenter(); // Will center the sprite by screen.
     * 
     * // Or notably
     * sprite.screenCenter(AXES_X); // Centers the x itself.
     * ```
     */
    void screenCenter(axes pos = AXES_XY);

    /**
     * @brief Frees sprite's specific variables and cleans up memory so you can handle making more rendered sprites.
     * 
     * For Null Safety reasons, there's a specific variable that's not gonna be freed but will stop you from rendering, or doing sprite's stuff.
     */
    void destroy();
};
} // namespace dsge

#endif