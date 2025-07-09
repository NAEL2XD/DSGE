#include "touch.hpp"

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
}
}