#ifndef __KML_GRAPHICS_H
#define __KML_GRAPHICS_H

#include <glm/glm.hpp>
#include <glad/glad.h>

static float LOG_SCREEN_WIDTH = 1080.0f, LOG_SCREEN_HEIGHT = 720.0f, LOG_SCREEN_ASPECT = LOG_SCREEN_WIDTH/LOG_SCREEN_HEIGHT;

namespace __KML::Shader {
    struct Shader {
        unsigned int id;

    };

    GLuint compile_src(GLenum type, const char* src, GLuint program);
    Shader create_program(const char* v_src, const char* f_src);
}

void kml__tempPC();
void kml__drawVertices(glm::mat4& model, unsigned int, glm::vec4 color);
unsigned int kml__getTex(std::string name);

#endif