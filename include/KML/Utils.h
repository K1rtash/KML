#ifndef KML_UTILS_H
#define KML_UTILS_H

#include "Vector.h"

#include <string>
#include <filesystem>

namespace KML 
{
    template<typename T>
    /**
     * @brief Clamps a value
     * @param x Value to be clamped, type @ref T
     * @param min Minimum value
     * @param max Maximum value
     * @returns @ref T
     */
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

    /**
     * @brief Returns a @ref Vec4f representing a RGBA color
     * @param R Red value, from 0 to 255
     * @param G Green value, from 0 to 255
     * @param B Blue value, from 0 to 255
     * @param A Alpha value, from 0 to 255
     * @returns @ref Vec4f
     *
     */
    inline Vec4f RGBA_v4f(int R, int G, int B, int A = 255) {
        return {(float)Clamp<int>(R, 0, 255), (float)Clamp<int>(G, 0, 255), (float)Clamp<int>(B, 0, 255), (float)Clamp<int>(A, 0, 255)};
    }

    /**
     * @brief Returns a @ref Vec3f representing a HSV color
     * @param H Hue in degrees, ranging from 0 to 360
     * @param S Saturation in %, ranging from 0 to 100
     * @param V Brightness in %, ranging from 0 to 100
     * @returns @ref Vec3f
     *
     */
    inline Vec3f HSV_v3f(int H, int S = 100, int V = 100) {
        return {(float)H, (float)S, (float)V};
    }

    inline void v3f_HSV(const Vec3f& __v, int* H, int* S, int* V) {
        *H = (int)(__v.x * 100.0f);
        *S = (int)(__v.y * 100.0f);
        *V = (int)(__v.z * 100.0f);
    }

    /**
     * @brief Changes HSV color to RGBA
     * 
     * Format is NORMALIZED, from 0-1
     * 
     * @param __v @ref HSV Vec3f
     * @returns RGBA @ref Vec4f
     */
    Vec4f HSVtoRGBA(const Vec3f& __v);

    /**
     * @brief Changes RGBA color to HSV
     * 
     * Format is human readable, ranging from 0-255
     * 
     * @param c RGBA @ref Vec4f
     * @returns HSV @ref Vec3f
     */
    Vec3f RGBAtoHSV(const Vec4f& c);

    /**
     * @brief Reads a file from disc
     * @param path Relative or absolute path
     * @returns String containing all file contents
     */
    std::string ReadFile(std::filesystem::path path);

    /**
     * @brief Generates a random number
     */
    float RandFloat(float min, float max);
}
#endif