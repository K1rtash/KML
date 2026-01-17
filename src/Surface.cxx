#include <string>
#include <iostream>
#include <format>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "__KML/graphics.h"
#include "KML/Surface.h"
#include "KML/Vector.h"
#include "KML/Utils.h"

using namespace KML;

Surface::Surface() {}

Surface::Surface(Vec2f pos, Vec2f scale) : pos{pos}, scale{scale}, rotation{scale.x / 2, scale.y / 2, 0.0f}  {}

Surface::Surface(std::string texture) {
    SetTexture(texture);
}

Surface::Surface(std::string texture, Vec2f pos) : pos{pos} {
    SetTexture(texture);
}
Surface::Surface(std::string texture, Vec4f transform) : pos{transform.x, transform.y}, scale{transform.z, transform.w}, rotation{scale.x / 2, scale.y / 2, 0.0f} {
    SetTexture(texture);
}

Surface::Surface(std::string texture, Vec4f transform, Vec3f rotation) : pos{transform.x, transform.y}, scale{transform.z, transform.w}, rotation{rotation} {
    SetTexture(texture);
}

Surface::Surface(std::string texture, float xPos, float yPos, float width, float height, float rotationDeg, float rotationAnchorX, float rotationAnchorY) 
: pos{xPos, yPos}, scale{width, height}, rotation{rotationAnchorX, rotationAnchorY, rotationDeg}
{
    SetTexture(texture);
}

void Surface::Draw() {
    glm::mat4 model = glm::mat4(1.0f);

    // 1. Trasladar al punto donde quieres dibujar
    model = glm::translate(model, glm::vec3(pos.x, pos.y, 0.0f));

    // 2. Trasladar al anchor point
    model = glm::translate(model, glm::vec3(rotation.x * scale.x, rotation.y * scale.y, 0.0f));

    // 3. Rotar alrededor del eje Z (rotación 2D)
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // 4. Volver desde el anchor point
    model = glm::translate(model, glm::vec3(-rotation.x * scale.x, -rotation.y * scale.y, 0.0f));

    // 5. Escalar (siempre después de rotar)
    model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));

    Vec4f clr = HSVtoRGBA(color);
    __KML::Shader::drawRect(model, glm::vec4{clr.x, clr.y, clr.z, clr.w}, tex);
}



void Surface::SetTexture(unsigned int id) {
    tex = id;
}

void Surface::SetTexture(std::string texture) {
    this->tex = kml__getTex(texture);
}


void Surface::SetColor_RGBA(const Vec4f& __v) {
    //color = Clamp(__v, 0.0f, 1.0f);
}

void Surface::SetColor_HSV(const Vec3f& __v) {
    color = __v;
}