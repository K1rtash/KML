#ifndef KML_SHAPE_H
#define KML_SHAPE_H

/**
 * @file    Shape.h
 * @brief   Default and user defined shapes to create surfaces
 */ 

#include "Graphics.h"

namespace KML {

    /**
     * @class      Shape Shape.h Shape
     * @brief      Used to set diferent shapes to render surfaces
     */
    class Shape {
      private:
        unsigned int vao;
        size_t ind_size;
      public:
        /**
         * @brief      Constructs a new instance.
         *
         * @param[in]      vertices       Array of vertices composed of Pos(x, y ,z = 0.0f) and UV Normals(x, y)
         * @param[in]  vertices_size  Size of the vertices array
         * @param[in]      indices        Array of indexed vertices to reuse 
         * @param[in]  indices_size   Size of the indices array
         */
        Shape(float* vertices, size_t vertices_size, unsigned int* indices, size_t indices_size);
        void Delete();

        /**
         * @brief      Called after each draw instance to render with the current shader program
         */
        void Use();
        ~Shape();
    };
}


#endif