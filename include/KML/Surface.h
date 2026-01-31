#ifndef KML_SURFACE_H
#define KML_SURFACE_H

/**
 * @file    Surface.h
 * @brief   Abstracts GL rendering as a @ref Shape, a @ref Texture, and a @ref Shader 
 */

#include "Vector.h"
#include "Graphics.h"
#include "Shape.h"
#include "ABI.h"

#include <string>

namespace KML {
    class KML_API Camera {
      public:
        Vec2f pos;
        Vec2f size;
        float rotation;
        Camera();
        Camera(Vec2f pos, Vec2f scale, float rotation);
    };

    struct KML_API Surface {
      Vec2f pos{0.0f, 0.0f};
      Vec2f scale{1.0f, 1.0f};
      Vec2f anchor{0.0f, 0.0f}; //0,0 es el centro
      Vec3f color{0.0f, 0.0f, 100.0f};
      int transparency = 0;
      float rotation = 0.0f;

      Surface() = default;

      Surface(float x, float y, float w, float h) : pos(x, y), scale(w, h) {}
    };

    class KML_API Sprite : public Surface {
      public:
        Shader* shader = nullptr;
        Shape* shape = nullptr;
        Camera* camera = nullptr;
        Texture tex = 0;

        void SetTexture(std::string texture);
        void SetColor_HSV(int H, int S = 100, int V = 100);
        void SetColor_RGB(int R, int G, int B);

        virtual void Draw();

        Sprite();
        Sprite(Shader* shader);
        Sprite(Vec4f transform);
        Sprite(std::string texture);
        Sprite(std::string texture, Vec4f transform);
    };
}

#endif