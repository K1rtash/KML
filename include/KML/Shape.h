#ifndef KML_SHAPE_H
#define KML_SHAPE_H

#include "Graphics.h"

namespace KML {
    class Shape {
      private:
        unsigned int vao;
        size_t ind_size;
      public:
        Shape(float* vertices, size_t vertices_size, unsigned int* indices, size_t indices_size);
        ~Shape();
        void Use();
    };
}


#endif