#include "dsge.hpp"
#include <iostream>
#include <thread>

#define col(a) 255 | (255 << (u32)8) | (255 << (u32)16) | (a << (u32)24)

namespace dsge {
// Public exports
color dsgeColor;
u64 elapsed;

namespace _internal {
    // Implementation details
    std::vector<Text> _debugText = {};
    std::vector<u8> _debugCol = {};
    C3D_RenderTarget* top = nullptr;

    void _logger(const std::string& message) {
        std::cout << message << std::endl;

        Text d(2, 2 + (11 * _debugText.size()), message);
        d.scale.set(0.4, 0.4);
        d._private.debug = true; // <-- Set debug mode
        _debugText.insert(_debugText.begin(), d);
        _debugCol.insert(_debugCol.begin(), 255);
    }

    void _renderDebugText() {
        int _index = 0;
        for (auto &&texts : _debugText) {
            texts.y = 2 + (11 * _index);
            texts.color = col(_debugCol[_index]);
            texts.render();
            _debugCol[_index]--;

            if (_debugCol[_index] == 0) {
                _debugText.erase(_debugText.begin() + _index);
                _debugCol.erase(_debugCol.begin() + _index);
                // Don't increase _index, continue with next item at this index
                continue;
            }

            if (_index == 22) {
                break;
            }
            _index++;
        }

        // Remove any extra entries beyond 22 for lag prevention
        if (_debugText.size() > 22) {
            _debugText.resize(22);
            _debugCol.resize(22);
        }
    }
}

void init() {
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);
    cfguInit();
    newsInit();
    romfsInit();
    ndspInit();
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Prepare();

    _internal::top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    
    dsgeColor.black       = 0xff000000;
    dsgeColor.blue        = 0xffff0000;
    dsgeColor.brown       = 0xff13458b;
    dsgeColor.cyan        = 0xffffff00;
    dsgeColor.gray        = 0xff808080;
    dsgeColor.green       = 0xff008000;
    dsgeColor.lime        = 0xff00ff00;
    dsgeColor.magenta     = 0xffff00ff;
    dsgeColor.orange      = 0xff00a5ff;
    dsgeColor.pink        = 0xffcbc0ff;
    dsgeColor.purple      = 0xff800080;
    dsgeColor.red         = 0xff0000ff;
    dsgeColor.transparent = 0x00000000;
    dsgeColor.white       = 0xffffffff;
    dsgeColor.yellow      = 0xff00ffff;
    
    srand(time(NULL));

    dsge::Sound::init();
    dsge::Text::init();
}

void render(std::function<void()> function) {
    u64 start = osGetTime();

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(_internal::top, C2D_Color32(0, 0, 0, 0xFF));
    C2D_SceneBegin(_internal::top);

    function();

    #if defined(DEBUG)
    _internal::_renderDebugText();
    #endif
    
    C3D_FrameEnd(0);

    elapsed = osGetTime() - start;
}

bool overlap(dsge::Sprite* obj1, dsge::Sprite* obj2) {
    if (!obj1->visible || !obj2->visible) {
        return false;
    }

    float width1 = obj1->width * fabsf(obj1->scale.x);
    float height1 = obj1->height * fabsf(obj1->scale.y);
    float width2 = obj2->width * fabsf(obj2->scale.x);
    float height2 = obj2->height * fabsf(obj2->scale.y);

    float left1 = obj1->flipX ? obj1->x - width1 : obj1->x;
    float top1 = obj1->flipY ? obj1->y - height1 : obj1->y;
    float right1 = left1 + width1;
    float bottom1 = top1 + height1;

    float left2 = obj2->flipX ? obj2->x - width2 : obj2->x;
    float top2 = obj2->flipY ? obj2->y - height2 : obj2->y;
    float right2 = left2 + width2;
    float bottom2 = top2 + height2;
    return left1 < right2 && right1 > left2 && top1 < bottom2 && bottom1 > top2;
}

int exit() {
    // Free DS game engine resources FIRST!
    dsge::Text::exit();
    dsge::Sound::exit();

    // Now shut down libraries (reverse order of init)
    C3D_Fini();
    C2D_Fini();
    ndspExit();
    romfsExit();
    newsExit();
    cfguExit();
    gfxExit();

    return 0;
}
}