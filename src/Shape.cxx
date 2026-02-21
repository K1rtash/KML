#include <glad/glad.h>

#include <cassert>

#include "KML/Shape.h"

struct KML::Shape {
    GLuint vao = 0, vbo = 0, ebo = 0;
    GLsizei count = {}; 
};

KML::Shape* KML::CreateShape(float* vertices, size_t vertSize, unsigned int* indices, size_t indSize) {
    Shape* shape = new Shape;
    assert(shape && vertices && vertSize > 0 && indices && indSize > 0);

    shape->count = indSize/sizeof(unsigned int);

    glGenVertexArrays(1, &(shape->vao));
    glGenBuffers(1, &(shape->vbo));
    glGenBuffers(1, &(shape->ebo));

    glBindVertexArray(shape->vao);

    glBindBuffer(GL_ARRAY_BUFFER, shape->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize, indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    assert(shape->vbo && shape->ebo && shape->count && shape->vao);

    return shape;
}

void KML::DeleteShape(Shape* shape) {
    assert(shape && shape->vao);

    glDeleteVertexArrays(1, &(shape->vao));
        glDeleteBuffers(1, &(shape->vbo));
    glDeleteBuffers(1, &(shape->ebo));
    delete shape;
}

void KML::DrawShape(Shape* shape) {
    assert(shape && shape->vao && shape->count);
    glBindVertexArray(shape->vao);
    glDrawElements(GL_TRIANGLES, shape->count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void KML::VertexFloatAttribute(Shape* shape, int index, int attributeSize, int stride, int offset) {
    assert(shape && index >= 0 && attributeSize > 0 && stride > 0 && offset >= 0);

    glBindVertexArray(shape->vao);
    glVertexAttribPointer(index, attributeSize, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
    glEnableVertexAttribArray(index);
    glBindVertexArray(0);
}