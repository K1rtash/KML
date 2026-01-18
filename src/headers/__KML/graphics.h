#ifndef __KML_GRAPHICS_H
#define __KML_GRAPHICS_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <unordered_map>

#include "KML/Vector.h"
#include "KML/Graphics.h"
#include "KML/Shape.h"

namespace __KML {
    extern float LOG_SCREEN_WIDTH, LOG_SCREEN_HEIGHT, LOG_SCREEN_ASPECT;
    extern KML::Shape* defaultShape;
    extern KML::Shader* defaultShader;
    void GenerateDefaultMembers();
}

namespace __KML::Texture {
    unsigned int load(const char* file);
    unsigned int get(std::string name);
}

#endif