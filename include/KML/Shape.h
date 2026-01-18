#ifndef KML_SHAPE_H
#define KML_SHAPE_H

#include "Graphics.h"

namespace KML::Shape {
    class __shape {
      private:
        unsigned int vao;
        size_t ind_size;
      public:
        __shape(float* vertices, size_t vs, unsigned int* indices, size_t is);
        ~__shape();
        void Use();
    };
    class 
}


#endif