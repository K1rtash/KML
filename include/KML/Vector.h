#ifndef KML_VECTOR_H
#define KML_VECTOR_H

namespace KML {
    struct Vec2f {
            float x, y;

            Vec2f operator+(const Vec2f& __v) const { return {x + __v.x, y + __v.y }; }
            Vec2f operator-(const Vec2f& __v) const { return {x - __v.x, y - __v.y }; }
            Vec2f operator*(const Vec2f& __v) const { return {x * __v.x, y * __v.y }; }
            Vec2f operator/(const Vec2f& __v) const { return {x / __v.x, y / __v.y }; }
            
            void operator+=(const Vec2f& __v) { x += __v.x; y += __v.y; }
            void operator-=(const Vec2f& __v) { x -= __v.x; y -= __v.y; }
            void operator*=(const Vec2f& __v) { *this = {x * __v.x, y * __v.y }; }
            void operator/=(const Vec2f& __v) { *this = {x / __v.x, y / __v.y }; }

            bool operator==(const Vec2f& __v) const { return x == __v.x && y == __v.y; }
            bool operator!=(const Vec2f& __v) const { return !(*this == __v); }

            void rotate(float angleDeg, const Vec2f& anchor);
            void normalize();
        };

        struct Vec3f {
            float x, y, z;

            Vec3f operator+(const Vec3f& __v) const { return {x + __v.x, y + __v.y, z + __v.z }; }
            Vec3f operator-(const Vec3f& __v) const { return {x - __v.x, y - __v.y, z - __v.z }; }
            Vec3f operator*(const Vec3f& __v) const { return {x * __v.x, y * __v.y, z * __v.z }; }
            Vec3f operator/(const Vec3f& __v) const { return {x / __v.x, y / __v.y, z / __v.z }; }
            
            void operator+=(const Vec3f& __v) { x += __v.x; y += __v.y, z += __v.z; }
            void operator-=(const Vec3f& __v) { x -= __v.x; y -= __v.y, z -= __v.z; }
            void operator*=(const Vec3f& __v) { *this = {x * __v.x, y * __v.y, z * __v.z }; }
            void operator/=(const Vec3f& __v) { *this = {x / __v.x, y / __v.y, z / __v.z }; }

            bool operator==(const Vec3f& __v) const { return x == __v.x && y == __v.y && z == __v.z; }
            bool operator!=(const Vec3f& __v) const { return !(*this == __v); }

            void normalize();
        };

        struct Vec4f {
            float x, y, z, w;

            Vec4f operator+(const Vec4f& __v) const { return {x + __v.x, y + __v.y, z + __v.z, w + __v.w }; }
            Vec4f operator-(const Vec4f& __v) const { return {x - __v.x, y - __v.y, z - __v.z, w - __v.w }; }
            Vec4f operator*(const Vec4f& __v) const { return {x * __v.x, y * __v.y, z * __v.z, w * __v.w }; }
            Vec4f operator/(const Vec4f& __v) const { return {x / __v.x, y / __v.y, z / __v.z, w / __v.w }; }
            
            void operator+=(const Vec4f& __v) { x += __v.x; y += __v.y, z += __v.z, w += __v.w; }
            void operator-=(const Vec4f& __v) { x -= __v.x; y -= __v.y, z -= __v.z, w -= __v.w; }
            void operator*=(const Vec4f& __v) { x *= __v.x; y *= __v.y, z *= __v.z, w *= __v.w; }
            void operator/=(const Vec4f& __v) { x /= __v.x; y /= __v.y, z /= __v.z, w /= __v.w; }

            bool operator==(const Vec4f& __v) const { return x == __v.x && y == __v.y && z == __v.z && w == __v.w; }
            bool operator!=(const Vec4f& __v) const { return !(*this == __v); }

            void normalize();
        };
}

#endif