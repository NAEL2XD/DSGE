#include "random.hpp"

namespace dsge {
namespace Random {
int integer(int min, int max) {
    return min + (rand() % (max - min + 1));
}

float floating(float min, float max, int decimal) {
    // Handle invalid ranges
    if (min > max) {
        float temp = min;
        min = max;
        max = temp;
    }
    if (min == max) return min;
    
    // Calculate scaling factor for decimals
    float factor = 1.0f;
    if (decimal > 0) {
        factor = powf(10.0f, (float)decimal);
    }
    
    // Generate Random float in range
    float range = max - min;
    float Random = min + (float)rand() / ((float)RAND_MAX / range);
    
    // Apply decimal precision
    if (decimal > 0) {
        Random = roundf(Random * factor) / factor;
    }
    
    // Ensure value stays within bounds
    if (Random < min) Random = min;
    if (Random > max) Random = max;
    
    return Random;
}

bool boolean(float chance) {
    return floating(0, 100, 4) < chance;
}

u32 color() {
    return C2D_Color32(integer(0, 255), integer(0, 255), integer(0, 255), 255);
}
}
}