#ifndef KML_SURFACE_H
#define KML_SURFACE_H

#include "Vector.h"
#include "Graphics.h"
#include "Shape.h"

#include <string>

namespace KML {
    class Camera {
      public:
        Vec2f pos;
        Vec2f size;
        float rotation;
        Camera();
        Camera(Vec2f pos, Vec2f scale, float rotation);
    };
    class Surface {
      public:
        Vec2f pos{0.0f, 0.0f};
        Vec2f scale{1.0f, 1.0f};
        Vec2f anchor{0.0f, 0.0f}; //0,0 es el centro
        Vec3f color{0.0f, 0.0f, 100.0f};

        Shader* shader = nullptr;
        Shape* shape = nullptr;
        Camera* camera = nullptr;

        int transparency = 0;
        float rotation = 0.0f;

        void SetTexture(std::string texture);
        void SetColor_HSV(int H, int S = 100, int V = 100);
        void SetColor_RGB(int R, int G, int B);

        virtual void Draw();

        Surface();
        Surface(Shader* shader);
        Surface(Vec4f transform);
        Surface(std::string texture);
        Surface(std::string texture, Vec4f transform);
        Surface(std::string texture, Vec4f transform, Vec2f anchor);
        Surface(std::string texture, Vec4f transform, Vec2f anchor, float rotation);
      protected:
        unsigned int tex = 0;
    };
}

#endif