#include <3ds.h>
#include "text.hpp"
#include "dsge.hpp"

C2D_Text c2dText;
namespace dsge {

// Static member initialization
C2D_Font Text::defaultFont = nullptr;
C2D_TextBuf Text::g_staticBuf = nullptr;

// Helper to combine alpha with color's alpha
u32 Text::applyAlpha(u32 color, float alpha) {
    u8 a = (color >> 24) & 0xFF;
    a = static_cast<u8>(a * alpha);
    return (color & 0x00FFFFFF) | (a << 24);
}

void Text::init() {
    defaultFont = C2D_FontLoadSystem(CFG_REGION_USA);
    g_staticBuf = C2D_TextBufNew(4096);
}

void Text::exit() {
    C2D_FontFree(Text::defaultFont);
    C2D_TextBufDelete(Text::g_staticBuf);
}

void Text::createText() {
    // Clear and prepare Text buffer
    C2D_TextBufClear(g_staticBuf);
    
    // Parse Text using selected font (default if none specified)
    C2D_TextFontParse(&c2dText, font ? font : defaultFont, g_staticBuf, text.c_str());
    C2D_TextOptimize(&c2dText);

    // Update width and height.
    C2D_TextGetDimensions(&c2dText, scale.x, scale.y, &width, &height);
}

Text::Text(int x, int y, const std::string& Text) :
    alignment(ALIGN_LEFT),
    alpha(1.0f),
    angle(0),
    borderStyle(BS_Border),
    bold(false),
    borderColor(0xFF000000),
    borderSize(0),
    color(0xFFFFFFFF),
    flipX(false),
    flipY(false),
    font(nullptr),
    height(0),
    text(Text),
    underline(false),
    visible(true),
    width(0),
    x(x), y(y),
    acceleration{0, 0},
    scale{1.0f, 1.0f}
{
    _private.debug = false;
    _private.destroyed = false;
    createText();
}

void Text::screenCenter(axes pos) {
    if (_private.destroyed) return;

    createText(); // Ensure width/height is updated

    float newX = (dsge::WIDTH  - width) / 2;
    float newY = (dsge::HEIGHT - height) / 2;

    switch(pos) {
        case AXES_X:   x = newX; break;
        case AXES_Y:   y = newY; break;
        case AXES_XY:  x = newX; y = newY; break;
    }
}

bool Text::isOnScreen() {
    if (_private.destroyed) return false;

    if (!visible) {
        return false;
    }

    // Making the text for reasons.
    createText();
    return !(x + width < 0 || x > dsge::WIDTH || y + height < 0 || y > dsge::HEIGHT);
}

void Text::render() {
    if (_private.destroyed || !visible || text.empty() || !isOnScreen()) return;

    createText(); // Ensure text dimensions are updated

    float scX = flipX ? -scale.x : scale.x;
    float scY = flipY ? -scale.y : scale.y;
    bool debug = _private.debug;

    x += acceleration.x;
    y += acceleration.y;

    float newX = x;
    if (!debug) {
        switch (alignment) {
            case ALIGN_LEFT:   break; // No change
            case ALIGN_CENTER: newX += (dsge::WIDTH - width) / 2; break;
            case ALIGN_RIGHT:  newX += dsge::WIDTH - width; break;
        }
    }

    C2D_ViewSave(&_private.matrix);
    C2D_ViewTranslate(newX, y);
    C2D_ViewRotate(debug ? 0 : Math::angleToRadians(angle));
    C2D_ViewScale(scX, scY);

    u32 col = applyAlpha(color, alpha);

    // Draw border effects
    if (borderSize != 0) {
        float border = fabsf(borderSize);
        u32 bCol = applyAlpha(borderColor, alpha);
        
        switch (borderStyle) {
            case BS_Border: {
                int offsets[8][2] = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}, {1, 0}, {-1, 0}, {0, 1}, {0, -1}};
                for (int i = 0; i < 8; i++) {
                    C2D_DrawText(&c2dText, C2D_WithColor, (offsets[i][0] * border), (offsets[i][1] * border), 0.5f, 1.0f, 1.0f, bCol);
                }
                break;
            }
            case BS_Shadow: {
                for (int i = 1; i < (int)borderSize + 1; i++) {
                    C2D_DrawText(&c2dText, C2D_WithColor, -i, i, 0.5f, 1.0f, 1.0f, bCol);
                }
                break;
            }
        }
    }

    // Bold
    if (bold) {
        for (int i = 1; i < 3; i++) {
            C2D_DrawText(&c2dText, C2D_WithColor, i, 0, 0.5f, 1.0f, 1.0f, col);
        }
    }

    // Underline
    if (underline) {
        C2D_DrawRectSolid(width + 2, height, 0.5f, width, 1.0f, col);
    }

    // Main text
    C2D_DrawText(&c2dText, C2D_WithColor, 0, 0, 0.5f, 1.0f, 1.0f, col);

    C2D_ViewRestore(&_private.matrix);
}

void Text::destroy() {
    if (_private.destroyed) return;
    
    if (font) {
        C2D_FontFree(font);
    }

    acceleration = {0, 0};
    alignment = ALIGN_LEFT;
    alpha = 0;
    angle = 0;
    borderStyle = BS_Border;
    bold = false;
    borderColor = 0;
    borderSize = 0;
    color = 0;
    flipX = false;
    flipY = false;
    font = nullptr;
    height = 0;
    scale = {0, 0};
    text = "";
    underline = false;
    visible = false;
    width = 0;
    x = 0;
    y = 0;

    _private.destroyed = true;
}
}