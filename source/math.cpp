#include "math.hpp"

namespace dsge {
namespace Math {
double distanceBetween(dsge::Sprite spriteA, dsge::Sprite spriteB) {
    int distX = spriteA.x - spriteB.x;
    int distY = spriteA.y - spriteB.y;
    return (double)sqrt(distX * distX + distY * distY);
}

float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

double angleToRadians(double angle) {
    return angle * (M_PI / 180);
}
}
}