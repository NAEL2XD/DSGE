#include "sprite.hpp"

namespace dsge {
Sprite::Sprite(int x, int y):
    alpha(1),
    angle(0),
    bottom(false),
    color(0xFFFFFFFF),
    flipX(false),
    flipY(false),
    height(0),
    visible(true),
    width(0),
    x(x), y(y),
    scale{1, 1},
    acceleration{0, 0}
{
    _private.image  = { NULL, NULL };
    _private.sprite = NULL;
    _private.destroyed = false;
}

bool Sprite::loadGraphic(const std::string& file) {
    if (_private.destroyed) return false;

    std::string filePath = "romfs:/" + file;

    _private.sprite = C2D_SpriteSheetLoad(filePath.c_str());
    if (!_private.sprite) {
        trace("[WARN] Sprite::loadGraphic: Failed to load Sprite sheet: " + file);
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

    float newW = (width * scale.x);

    float newX = bottom ? (dsge::WIDTH_BOTTOM - newW) / 2 : (dsge::WIDTH - newW) / 2;
    float newY = (dsge::HEIGHT - (height * scale.y)) / 2;

    switch(pos) {
        case AXES_X:  x = newX; break;
        case AXES_Y:  y = newY; break;
        case AXES_XY: x = newX; y = newY; break;
    }
}

bool Sprite::isOnScreen() {
    if (_private.destroyed || !visible) {
        return false;
    }

    return !(x + (width * scale.x) < 0 || x > dsge::WIDTH || y + (height * scale.y) < 0 || y > dsge::HEIGHT);
}

void Sprite::_render() {
    if (_private.destroyed || !visible || (width == 0 && height == 0) || !isOnScreen()) return;

    // Crash prevention(?)
    if (width < 0) width = -width;
    if (height < 0) height = -height;

    float scX = flipX ? -scale.x : scale.x;
    float scY = flipY ? -scale.y : scale.y;

    x += acceleration.x;
    y += acceleration.y;
    angle += acceleration.angle;

    C2D_ViewSave(&_private.matrix);

    C2D_ViewTranslate(x, y);
    C2D_ViewTranslate(width * scX / 2, height * scY / 2);
    C2D_ViewRotate(Math::angleToRadians(angle));
    C2D_ViewScale(scX, scY);

    if (_private.image.tex != NULL) {
        C2D_PlainImageTint(&_private.tint, C2D_Color32((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, ((color >> 24) & 0xFF) * (alpha >= 1 ? 1 : alpha <= 0 ? 0 : alpha)), 0);
        C2D_DrawImageAt(_private.image, -width / 2, -height / 2, 0, &_private.tint, 1, 1);
    } else {
        u32 finalColor = color;
        if (alpha < 1) {
            u8 a = (color >> 24) & 0xFF;
            a = (u8)(a * alpha);
            finalColor = (color & 0x00FFFFFF) | (a << 24);
        }
        C2D_DrawRectSolid(-width / 2, -height / 2, 0, width * fabsf(scX), height * fabsf(scY), finalColor);
    }

    C2D_ViewRestore(&_private.matrix);
}

void Sprite::destroy() {
    if (_private.destroyed) return;

    if (_private.sprite) {
        C2D_SpriteSheetFree(_private.sprite);
        _private.sprite = nullptr;
    }
    acceleration = {0, 0};
    angle = 0;
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