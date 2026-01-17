#ifndef __KML_GRAPHICS_H
#define __KML_GRAPHICS_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <unordered_map>

static float LOG_SCREEN_WIDTH = 1080.0f, LOG_SCREEN_HEIGHT = 720.0f, LOG_SCREEN_ASPECT = LOG_SCREEN_WIDTH/LOG_SCREEN_HEIGHT;

namespace __KML::Shader {
    struct Shader {
        unsigned int id;
        std::unordered_map<std::string, unsigned int> uniforms;
    };

    GLuint compile_src(GLenum type, const char* src, GLuint program);
    Shader create_program(const char* v_src, const char* f_src);
    void map_shader_uniforms(Shader* shader);
    GLuint get_uniform_loc(Shader* shader, std::string name);
    void drawRect(glm::mat4& model, glm::vec4 color, unsigned int tex);
}

void kml__tempPC();
void kml__drawVertices(glm::mat4& model, unsigned int, glm::vec4 color);
unsigned int kml__getTex(std::string name);

#endif