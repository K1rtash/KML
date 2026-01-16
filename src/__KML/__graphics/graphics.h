#ifndef __KML_GRAPHICS_H
#define __KML_GRAPHICS_H

#include <glm/glm.hpp>

static float LOG_SCREEN_WIDTH = 1080.0f, LOG_SCREEN_HEIGHT = 720.0f;

void kml__shader_setup();
void kml__drawVertices(glm::mat4& model, unsigned int, glm::vec4 color);
unsigned int kml__getTex(std::string name);

#endif