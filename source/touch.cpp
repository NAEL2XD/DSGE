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
    return getTouchData().px;
}

u16 Touch::getTouchY() {
    if (!isTouchHeld()) return -1;
    return getTouchData().py;
}

template<typename T>
bool Touch::isTouching_impl(T& obj) {
    if (!obj.visible || !obj.bottom || !isTouchHeld()) {
        return false;
    }

    touchPosition t = getTouchData();
    return (obj.x < t.px) && (obj.x + (obj.width * obj.scale.x) > t.px) && (obj.y < t.py) && (obj.y + (obj.height * obj.scale.y) > t.py);
}

bool Touch::isTouching(dsge::Sprite& obj) {
    return isTouching_impl(obj);
}

bool Touch::isTouching(dsge::Text& obj) {
    return isTouching_impl(obj);
}
} // namespace dsge