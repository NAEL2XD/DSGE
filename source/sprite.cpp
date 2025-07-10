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
    _private.sprite = NULL;
    _private.destroyed = false;
}

bool Sprite::loadGraphic(const std::string& file) {
    if (_private.destroyed) return false;

    std::string filePath = "romfs:/" + file;

    _private.sprite = C2D_SpriteSheetLoad(filePath.c_str());
    if (!_private.sprite) {
        print("[WARN] Sprite::loadGraphic: Failed to load Sprite sheet: " + file);
        return false;
    }

    C2D_Image ret = C2D_SpriteSheetGetImage(_private.sprite, 0);
    _private.image = ret;
    width = ret.subtex->width;
    height = ret.subtex->height;

    return true;
}

void Sprite::makeGraphic(int width, int height, u32 color) {
    if (_private.destroyed) return;

    this->width  = fabs(width);
    this->height = fabs(height);
    this->color  = fabs(color);
}

void Sprite::screenCenter(axes pos) {
    if (_private.destroyed) return;

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
    if (_private.destroyed || !visible) {
        return false;
    }

    return !(x + width < 0 || x > dsge::WIDTH || y + height < 0 || y > dsge::HEIGHT);
}

void Sprite::render() {
    if (_private.destroyed || !visible || (width == 0 && height == 0) || !isOnScreen()) return;

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

void Sprite::destroy() {
    if (_private.destroyed) return;

    if (_private.sprite) {
        C2D_SpriteSheetFree(_private.sprite);
        _private.sprite = nullptr;
    }

    acceleration = {0, 0};
    angle = 0.0f;
    color = 0;
    flipX = false;
    flipY = false;
    scale = {0, 0};
    visible = false;
    width = 0;
    height = 0;
    x = 0;
    y = 0;

    _private.image = {nullptr, nullptr};
    _private.destroyed = true;
}
} // namespace dsge