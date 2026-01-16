#ifndef KML_UTILS_H
#define KML_UTILS_H

#include "Vector.h"

namespace KML 
{
    template<typename T>
    inline T Clamp(T x, T min = (T)0.0f, T max = (T)1.0f) { 
        if (x < min) return min; 
        if (x > max) return max; 
        return x;
    }

    inline Vec2f Clamp(const Vec2f& __v, float min = 0.0f, float max = 1.0f) {
        return {Clamp<float>(__v.x, min, max), Clamp<float>(__v.y, min, max)};
    }
    inline Vec3f Clamp(const Vec3f& __v, float min = 0.0f, float max = 1.0f) {
        return {Clamp<float>(__v.x, min, max), Clamp<float>(__v.y, min, max), Clamp<float>(__v.z, min, max)};
    }

    inline Vec4f Clamp(const Vec4f& __v, float min = 0.0f, float max = 1.0f) {
        return {Clamp<float>(__v.x, min, max), Clamp<float>(__v.y, min, max), Clamp<float>(__v.z, min, max), Clamp<float>(__v.w, min, max)};
    }

    inline Vec4f RGBA_v4f(int R, int G, int B, int A = 255) {
        return {(float)Clamp<int>(R, 0, 255) / 255.0f, (float)Clamp<int>(G, 0, 255) / 255.0f, (float)Clamp<int>(B, 0, 255) / 255.0f, (float)Clamp<int>(A, 0, 255) / 255.0f};
    }

    inline Vec3f HSV_v3f(float H, float S = 1.0f, float V = 1.0f) {
        return {Clamp<float>(H, 0.0f, 360.0f), Clamp<float>(S, 0.0f, 1.0f), Clamp<float>(V, 0.0f, 1.0f)};
    }

    Vec4f HSVtoRGBA(const Vec3f& __v);
    Vec3f RGBAtoHSV(const Vec4f& c);
}
#endif