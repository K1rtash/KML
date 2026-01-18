#ifndef __KML_GRAPHICS_H
#define __KML_GRAPHICS_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <unordered_map>

#include "KML/Vector.h"
#include "KML/Graphics.h"

namespace __KML {
    extern float LOG_SCREEN_WIDTH, LOG_SCREEN_HEIGHT, LOG_SCREEN_ASPECT;
    extern GLuint VAO;
    extern KML::Shader* program0;
    extern KML::Shader* program1;

    KML::Shader* create_program_from_src(std::string v_src, std::string f_src);
    GLuint get_program_id(KML::Shader* shader);
}

namespace __KML::Texture {
    unsigned int load(const char* file);
    unsigned int get(std::string name);
}

void kml__tempPC();

#endif