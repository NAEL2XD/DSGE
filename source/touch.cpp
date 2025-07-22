#include "touch.hpp"
#include "sprite.hpp"
#include "text.hpp"

namespace dsge {
void Touch::update() {
    hidScanInput();
}

touchPosition Touch::getTouchData() {
    touchPosition t;
    hidTouchRead(&t);
    return t;
}

bool Touch::isTouchHeld() {
    update();
    return hidKeysHeld() & KEY_TOUCH;
}

bool Touch::isTouchDown() {
    update();
    return hidKeysDown() & KEY_TOUCH;
}

bool Touch::isTouchUp() {
    update();
    return hidKeysUp() & KEY_TOUCH;
}

u16 Touch::getTouchX() {
    if (!isTouchHeld()) return -1;
    touchPosition t = getTouchData();
    return t.px;
}

u16 Touch::getTouchY() {
    if (!isTouchHeld()) return -1;
    touchPosition t = getTouchData();
    return t.py;
}

template<typename T>
bool Touch::isTouching_impl(T& obj) {
    if (!obj.visible) {
        return false;
    }

    if (!isTouchHeld()) {
        return false;
    }

    touchPosition t = getTouchData();
    return (obj.x < t.px) && (obj.x + obj.width > t.px) && (obj.y < t.py) && (obj.y + obj.height > t.py);
}

bool Touch::isTouching(dsge::Sprite& obj) {
    return isTouching_impl(obj);
}

bool Touch::isTouching(dsge::Text& obj) {
    return isTouching_impl(obj);
}
} // namespace dsge