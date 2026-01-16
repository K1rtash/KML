#include "KML/Vector.h"

#include <cmath>

using namespace KML;

void Vec2f::rotate(float angleDeg, const Vec2f& anchor) {
    float rad = angleDeg * 3.14159265f / 180.0f;
    float c = std::cos(rad), s = std::sin(rad);
    float tx = x - anchor.x;
    float ty = y - anchor.y;
    *this = {tx * c - ty * s + anchor.x, tx * s + ty * c + anchor.y};
}

void Vec2f::normalize() {
    float r = std::sqrt(x*x + y*y); 
    if (r != 0.0f) { 
        x /= r; 
        y /= r; 
    }
}

void Vec3f::normalize() {
    float r = std::sqrt(x*x + y*y + z*z); 
    if (r != 0.0f) { 
        x /= r; 
        y /= r; 
        z /= r;
    }
}

void Vec4f::normalize() {
    float r = std::sqrt(x*x + y*y + z*z + w*w); 
    if (r != 0.0f) { 
        x /= r; 
        y /= r; 
        z /= r;
        w /= w;
    }
}