#include <glad/glad.h>

#include "KML/Shape.h"

#include "__KML/graphics.h"

KML::Shape::Rect::Rect(float* vertices, size_t vs, unsigned int* indices, size_t is) {
    GLuint VBO, EBO;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vs, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, is, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    this->ind_size = is;

    assert(vao != 0);
}

KML::Shape::Rect::~Rect() {
    if(vao) glDeleteVertexArrays(1, &vao);
}

void KML::Shape::Rect::Use() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, ind_size/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
    glUseProgram(0);
    glBindVertexArray(0);
}