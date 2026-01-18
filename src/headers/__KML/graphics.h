#ifndef __KML_GRAPHICS_H
#define __KML_GRAPHICS_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <unordered_map>

#include "KML/Vector.h"
#include "KML/Graphics.h"

static float LOG_SCREEN_WIDTH = 1080.0f, LOG_SCREEN_HEIGHT = 720.0f, LOG_SCREEN_ASPECT = LOG_SCREEN_WIDTH/LOG_SCREEN_HEIGHT;

namespace __KML {
    KML::Shader* create_program_from_src(std::string v_src, std::string f_src);
    unsigned int get_program_id(KML::Shader* shader);
    void draw_rect(glm::mat4& model, KML::Vec4f color, KML::Shader* shader);
    void draw_rect(glm::mat4& model, KML::Vec4f color, KML::Shader* shader, unsigned int tex);
    void draw_rect(KML::Shader* shader);
}

namespace __KML::Texture {
    unsigned int load(const char* file);
    unsigned int get(std::string name);
}

void kml__tempPC();

#endif