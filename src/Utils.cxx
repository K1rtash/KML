#include "KML/Utils.h"

#include <algorithm>
#include <iostream>
#include <fstream>

using namespace KML;
using fs = std::fstream;

Vec4f KML::HSVtoRGBA(const Vec3f& __v) {
    float H = Clamp<float>(__v.x, 0.0f, 360.0f) / 360.0f;
    float S = Clamp<float>(__v.y, 0.0f, 100.0f) / 100.0f;
    float V = Clamp<float>(__v.z, 0.0f, 100.0f) / 100.0f;

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
    float r = Clamp<float>(c.x, 0.0f, 255.0f) / 255.0f;
    float g = Clamp<float>(c.y, 0.0f, 255.0f) / 255.0f;
    float b = Clamp<float>(c.z, 0.0f, 255.0f) / 255.0f;

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

    return Vec3f{H, S * 100.0f, V * 100.0f}; // HSV humano
}

std::string KML::ReadFile(const char* path) {
    std::ifstream file(path);
    if(file.is_open()) {
        std::string str;
        std::string out;
        while(std::getline(file, str)) {
            out += str + '\n';
        }
        return out;
    }
    return {0};
}
