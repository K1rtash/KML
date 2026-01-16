#include "KML/Utils.h"

#include <algorithm>

using namespace KML;

Vec4f KML::HSVtoRGBA(const Vec3f& __v) {
    float H = Clamp<float>(__v.x, 0.0f, 360.0f) / 360.0f;
    float S = __v.y;
    float V = __v.z;

    int i = int(H * 6);
    float f = H * 6 - i;
    float p = V * (1 - S);
    float q = V * (1 - f * S);
    float t = V * (1 - (1 - f) * S);

    switch(i % 6) {
        case 0: return Vec4f{V, t, p, 1.0f};
        case 1: return Vec4f{q, V, p, 1.0f};
        case 2: return Vec4f{p, V, t, 1.0f};
        case 3: return Vec4f{p, q, V, 1.0f};
        case 4: return Vec4f{t, p, V, 1.0f};
        case 5: return Vec4f{V, p, q, 1.0f};
    }
    return Vec4f{1.0f, 1.0f, 1.0f, 1.0f};
}

Vec3f KML::RGBAtoHSV(const Vec4f& c) {
    float r = c.x;
    float g = c.y;
    float b = c.z;

    float maxc = std::max(r, std::max(g, b));
    float minc = std::min(r, std::min(g, b));
    float delta = maxc - minc;

    float H = 0.0f;
    float S = 0.0f;
    float V = maxc;

    if (delta > 0.00001f) {
        S = delta / maxc;

        if (maxc == r) {
            H = (g - b) / delta;
        } else if (maxc == g) {
            H = 2.0f + (b - r) / delta;
        } else {
            H = 4.0f + (r - g) / delta;
        }

        H *= 60.0f;
        if (H < 0.0f)
            H += 360.0f;
    }
    return Vec3f{H, S, V};
}