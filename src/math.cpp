#include <SFML/Graphics.hpp>
#include <iostream>
#include "math.hpp"


float math::difference(float a, float b) {
    float offset = std::min(a,b);
    a += offset;
    b += offset;
    float diff = std::max(a,b) - std::min(a,b);
    return diff;
}
