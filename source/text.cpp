#include "text.hpp"

C2D_Text c2dText;
namespace dsge {

// Static member initialization
C2D_Font Text::defaultFont = NULL;
C2D_TextBuf Text::g_staticBuf = NULL;

// Helper to combine alpha with color's alpha
u32 Text::applyAlpha(u32 color, float alpha) {
    u8 a = (color >> 24) & 0xFF;
    a = static_cast<u8>(a * alpha);
    return (color & 0x00FFFFFF) | (a << 24);
}

void Text::exit() {
    C2D_FontFree(defaultFont);
    C2D_TextBufDelete(g_staticBuf);
}

void Text::createText() {
    // Clear and prepare Text buffer
    if (g_staticBuf == NULL) { // Fuck you null
        defaultFont = C2D_FontLoadSystem(CFG_REGION_USA);
        g_staticBuf = C2D_TextBufNew(4096);
    }
    C2D_TextBufClear(g_staticBuf);
    
    // Parse Text using selected font (default if none specified)
    C2D_TextFontParse(&c2dText, font ? font : defaultFont, g_staticBuf, text.c_str());
    C2D_TextOptimize(&c2dText);

    // Update width and height.
    C2D_TextGetDimensions(&c2dText, scale.x, scale.y, &width, &height);
}

Text::Text(int x, int y, const std::string& Text) :
    alignment(ALIGN_LEFT),
    alpha(1),
    angle(0),
    borderStyle(BS_BORDER),
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
    scale{1, 1}
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
    float newB = (dsge::WIDTH_BOTTOM - width) / 2;

    switch(pos) {
        case AXES_X:      x = newX; break;
        case AXES_Y:      y = newY; break;
        case AXES_XY:     x = newX; y = newY; break;
        case AXES_X_BOT:  x = newB; break;
        case AXES_Y_BOT:  break;
        case AXES_XY_BOT: x = newB; y = newY; break;
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

bool Text::loadFont(std::string filePath) {
    std::string fullPath = "romfs:/" + filePath;

    if (!std::filesystem::exists(fullPath)) {
        print("[WARN] Text::loadFont: File Path doesn't exist: " + fullPath);
        return false;
    }

    return (font = C2D_FontLoad(fullPath.c_str())) != NULL;
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
            case ALIGN_LEFT:       break; // No change
            case ALIGN_CENTER:     newX += (dsge::WIDTH - width) / 2; break;
            case ALIGN_RIGHT:      newX += dsge::WIDTH - width; break;
            case ALIGN_CENTER_BOT: newX = (dsge::WIDTH_BOTTOM - width) / 2; break;
            case ALIGN_RIGHT_BOT:  newX = dsge::WIDTH_BOTTOM - width; break;
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
            case BS_BORDER: {
                int offsets[8][2] = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}, {1, 0}, {-1, 0}, {0, 1}, {0, -1}};
                for (int i = 0; i < 8; i++) {
                    C2D_DrawText(&c2dText, C2D_WithColor, (offsets[i][0] * border), (offsets[i][1] * border), .5, 1, 1, bCol);
                }
                break;
            }
            case BS_SHADOW: {
                for (int i = 1; i < (int)borderSize + 1; i++) {
                    C2D_DrawText(&c2dText, C2D_WithColor, -i, i, .5, 1, 1, bCol);
                }
                break;
            }
        }
    }

    // Bold
    if (bold) {
        for (int i = 1; i < 3; i++) {
            C2D_DrawText(&c2dText, C2D_WithColor, i, 0, 0, 1, 1, col);
        }
    }

    // Underline
    if (underline) {
        C2D_DrawRectSolid(width + 2, height, .5, width, 1, col);
    }

    // Main text
    C2D_DrawText(&c2dText, C2D_WithColor, 0, 0, .5, 1, 1, col);

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
    borderStyle = BS_BORDER;
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