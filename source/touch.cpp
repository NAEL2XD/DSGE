#include "touch.hpp"
#include "sprite.hpp"

namespace dsge {
namespace Touch {
touchPosition t;
void update() {
    hidScanInput();
    hidTouchRead(&t);
}

bool isTouchHeld() {
    update();
    return hidKeysHeld() & KEY_TOUCH;
}

bool isTouchDown() {
    update();
    return hidKeysDown() & KEY_TOUCH;
}

bool isTouchUp() {
    update();
    return hidKeysUp() & KEY_TOUCH;
}

u16 getTouchX() {
    update();
    return hidKeysHeld() & KEY_TOUCH ? -1 : t.px;
}

u16 getTouchY() {
    update();
    return hidKeysHeld() & KEY_TOUCH ? -1 : t.py;
}

bool isTouching(dsge::Sprite* obj1) {
    if (!obj1->visible) {
        return false;
    }

    update();
    return obj1->x < t.px && obj1->x + obj1->width > t.px && obj1->y < t.py && obj1->y + obj1->height > t.py;
}
}
}