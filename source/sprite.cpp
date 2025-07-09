#include "sprite.hpp"
#include "dsge.hpp"

namespace dsge {
Sprite::Sprite(int x, int y) :
    angle(0),
    color(0xFFFFFFFF),
    flipX(false),
    flipY(false),
    height(0),
    visible(true),
    width(0),
    x(x), y(y),
    scale{1.0f, 1.0f},
    acceleration{0, 0}
{
    _private.image = { NULL, NULL };
}

bool Sprite::loadGraphic(const std::string& file) {
    std::string filePath = "romfs:/" + file;

    C2D_SpriteSheet spriteSheet = C2D_SpriteSheetLoad(filePath.c_str());
    if (!spriteSheet) {
        print("[WARN] Sprite::loadGraphic: Failed to load Sprite sheet: " + file);
        return false;
    }

    C2D_Image ret = C2D_SpriteSheetGetImage(spriteSheet, 0);
    _private.image = ret;
    width = ret.subtex->width;
    height = ret.subtex->height;

    return true;
}

void Sprite::makeGraphic(int width, int height, u32 color) {
    this->width  = fabs(width);
    this->height = fabs(height);
    this->color  = fabs(color);
}

void Sprite::screenCenter(axes pos) {
    float newX = ((dsge::WIDTH - width) / 2) + (width / 2);
    float newY = ((dsge::HEIGHT - height) / 2) + (height / 2);

    switch(pos) {
        case AXES_X: {
            x = newX;
        }
        case AXES_Y: {
            y = newY;
        }
        case AXES_XY: {
            x = newX;
            y = newY;
        }
    }
}

bool Sprite::isOnScreen() {
    if (!visible) {
        return false;
    }

    return !(x + width < 0 || x > dsge::WIDTH || y + height < 0 || y > dsge::HEIGHT);
}

void Sprite::render() {
    if (!visible || (width == 0 && height == 0) || !isOnScreen()) return;

    // Crash prevention(?)
    if (width < 0) width = -width;
    if (height < 0) height = -height;

    float scX = flipX ? -scale.x : scale.x;
    float scY = flipY ? -scale.y : scale.y;

    x += acceleration.x;
    y += acceleration.y;
    
    if (_private.image.tex != NULL) {
        C2D_DrawImageAtRotated(_private.image, x, y, 0, Math::angleToRadians(angle), NULL, scX, scY);
    } else {
        float renderWidth = width * fabsf(scX);
        float renderHeight = height * fabsf(scY);
        C2D_DrawRectSolid(x, y, 0, renderWidth, renderHeight, color);
    }
}

} // namespace dsge