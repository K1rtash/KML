#ifndef KML_SURFACE_H
#define KML_SURFACE_H

#include "Vector.h"

#include <string>

namespace KML {
    class Surface {
      public:
        Vec2f pos{1.0f, 1.0f};
        Vec2f scale{1.0f, 1.0f};
        Vec3f rotation{0.0f, 0.0f, 0.0f};
        Vec3f color{0.0f, 0.0f, 100.0f};
        bool flip_x = false, flip_y = false;

        void Draw();
        void SetTexture(std::string texture);
        void SetColor_RGBA(const Vec4f& __v);
        void SetColor_HSV(const Vec3f& __v);

        Surface();
        Surface(Vec2f pos, Vec2f scale); // Solo forma
        Surface(std::string texture);   // Solo textura
        Surface(std::string texture, Vec2f pos); // Textura y posicion
        Surface(std::string texture, Vec4f transform); // Textura posicion y figura
        Surface(std::string texture, Vec4f transform, Vec3f rotation); // Todo
        Surface(std::string texture, float xPos, float yPos, float width, float height, float rotationDeg, float rotationAnchorX, float rotationAnchorY);
    private:
        unsigned int tex = 0;
    };
}

#endif