#ifndef KML_SHAPE_H
#define KML_SHAPE_H

/**
 * @file    Shape.h
 * @brief   Default and user defined shapes to create surfaces
 */ 

#include "Graphics.h"
#include "ABI.h"

namespace KML {
    struct Shape;

    /**
     * @brief      Creates a shape.
     *
     * @param      vertices      Array of floats representing
     * @param[in]  verticesSize  Size of the array of vertices
     * @param      indices       Array of unsigned integers
     * @param[in]  indicesSize   Size of the array of indices
     *
     * @return     @ref Shape
     */
    Shape* CreateShape(float* vertices, size_t verticesSize, unsigned int* indices, size_t indicesSize);

    /**
     * @brief      Defines a single attribute in a vertex
     * 
     * Attributes are a number of consecutive floats in a vertex, you must specify where it starts in the vertex, and the distance between an attribute and
     * the next one in the consecutive vertex
     *
     * @param[in]  shape           @ref Shape
     * @param[in]  index           Location of the attribute in the layout
     * @param[in]  attributeSize   How many components does this attribute have
     * @param[in]  stride          Components between an attribute and the same attribute in the next vertex
     * @param[in]  offset          Components to advance from the start of a vertex to reach the attribute
     * 
     * In the vertex shader, you should import the vertices as a layout like this:
     * 
     * layout (location = INDEX) in NAME
     * 
     */
    void VertexFloatAttribute(Shape* shape, int index, int attributeSize, int stride, int offset);
    void DrawShape(Shape* shape);
    void DeleteShape(Shape* shape);
}


#endif
