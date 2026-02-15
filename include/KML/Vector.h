#ifndef KML_VECTOR_H
#define KML_VECTOR_H

/**
 * @file        Vector.h
 * @brief       Basic linear algebra abstractions 
 */

#include "ABI.h"

namespace KML {
    struct KML_API Vec2f {
            float x, y;

            Vec2f(float _x, float _y) : x{_x}, y{_y} {}
            Vec2f() : x(0), y(0) {}
            Vec2f(float v) : x(v), y(v) {}
            Vec2f(const Vec2f& _v) : x(_v.x), y(_v.y) {}

            Vec2f operator+(const Vec2f& __v) const { return {x + __v.x, y + __v.y }; }
            Vec2f operator-(const Vec2f& __v) const { return {x - __v.x, y - __v.y }; }
            Vec2f operator*(const Vec2f& __v) const { return {x * __v.x, y * __v.y }; }
            Vec2f operator/(const Vec2f& __v) const { return {x / __v.x, y / __v.y }; }

            Vec2f operator+(float t) const { return {x + t, y + t }; }
            Vec2f operator-(float t) const { return {x - t, y - t }; }
            Vec2f operator*(float t) const { return {x * t, y * t }; }
            Vec2f operator/(float t) const { return {x / t, y / t}; }
            
            void operator=(const Vec2f& _v) { x = _v.x; y = _v.y; }
            void operator+=(const Vec2f& __v) { x += __v.x; y += __v.y; }
            void operator-=(const Vec2f& __v) { x -= __v.x; y -= __v.y; }
            void operator*=(const Vec2f& __v) { x *= __v.x; y *= __v.y; }
            void operator/=(const Vec2f& __v) { x /= __v.x; y /= __v.y; };

            bool operator==(const Vec2f& __v) const { return x == __v.x && y == __v.y; }
            bool operator!=(const Vec2f& __v) const { return !(*this == __v); }

            void rotate(float angleDeg, const Vec2f& anchor);
            void normalize();
        };

        struct KML_API Vec3f {
            float x, y, z;

            Vec3f(float _x, float _y, float _z) : x{_x}, y{_y}, z{_z} {}
            Vec3f() : x(0), y(0), z(0) {}
            Vec3f(float v) : x(v), y(v), z(v) {}
            Vec3f(const Vec3f& _v) : x(_v.x), y(_v.y), z(_v.z) {}


            Vec3f operator+(const Vec3f& __v) const { return {x + __v.x, y + __v.y, z + __v.z }; }
            Vec3f operator-(const Vec3f& __v) const { return {x - __v.x, y - __v.y, z - __v.z }; }
            Vec3f operator*(const Vec3f& __v) const { return {x * __v.x, y * __v.y, z * __v.z }; }
            Vec3f operator/(const Vec3f& __v) const { return {x / __v.x, y / __v.y, z / __v.z }; }
            
            void operator=(const Vec3f& _v) { x = _v.x; y = _v.y; z = _v.z; }
            void operator+=(const Vec3f& __v) { x += __v.x; y += __v.y; z += __v.z; }
            void operator-=(const Vec3f& __v) { x -= __v.x; y -= __v.y; z -= __v.z; }
            void operator*=(const Vec3f& __v) { x *= __v.x; y *= __v.y; z *= __v.z; }
            void operator/=(const Vec3f& __v) { x /= __v.x; y /= __v.y; z /= __v.z; }

            bool operator==(const Vec3f& __v) const { return x == __v.x && y == __v.y && z == __v.z; }
            bool operator!=(const Vec3f& __v) const { return !(*this == __v); }

            void normalize();
        };

        struct KML_API Vec4f {
            float x, y, z, w;

            Vec4f(float _x, float _y, float _z, float _w) : x{_x}, y{_y}, z{_z}, w{_w} {}
            Vec4f() : x(0), y(0), z(0) {}
            Vec4f(float v) : x(v), y(v), z(v), w(v) {}
            Vec4f(const Vec4f& _v) : x(_v.x), y(_v.y), z(_v.z), w(_v.w) {}

            Vec4f operator+(const Vec4f& __v) const { return {x + __v.x, y + __v.y, z + __v.z, w + __v.w }; }
            Vec4f operator-(const Vec4f& __v) const { return {x - __v.x, y - __v.y, z - __v.z, w - __v.w }; }
            Vec4f operator*(const Vec4f& __v) const { return {x * __v.x, y * __v.y, z * __v.z, w * __v.w }; }
            Vec4f operator/(const Vec4f& __v) const { return {x / __v.x, y / __v.y, z / __v.z, w / __v.w }; }
            
            void operator=(const Vec4f& _v) { x = _v.x; y = _v.y; z = _v.z; w = _v.w; }
            void operator+=(const Vec4f& __v) { x += __v.x; y += __v.y, z += __v.z, w += __v.w; }
            void operator-=(const Vec4f& __v) { x -= __v.x; y -= __v.y, z -= __v.z, w -= __v.w; }
            void operator*=(const Vec4f& __v) { x *= __v.x; y *= __v.y, z *= __v.z, w *= __v.w; }
            void operator/=(const Vec4f& __v) { x /= __v.x; y /= __v.y, z /= __v.z, w /= __v.w; }

            bool operator==(const Vec4f& __v) const { return x == __v.x && y == __v.y && z == __v.z && w == __v.w; }
            bool operator!=(const Vec4f& __v) const { return !(*this == __v); }

            void normalize();
        };

        inline Vec2f operator+(float t, const Vec2f& v) { return v + t; }
        inline Vec2f operator-(float t, const Vec2f& v) { return v - t; }
        inline Vec2f operator*(float t, const Vec2f& v) { return v * t; }
        inline Vec2f operator/(float t, const Vec2f& v) { return v / t; }
}

#endif
