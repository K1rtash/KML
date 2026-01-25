#include <string>
#include <cassert>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "__KML/graphics.h"
#include "KML/Surface.h"
#include "KML/Vector.h"
#include "KML/Utils.h"
#include "KML/Graphics.h"

using namespace KML;

Surface::Surface() {
    pos = Vec2f{0, 0};
    scale = Vec2f{1, 1};
    anchor = Vec2f{0.0f, 0.0f}; //x y para anchor, z para angulo
    color = Vec3f{0.0f, 0.0f, 100.0f};
    rotation = 0.0f;
}

Surface::Surface(Shader* shader) : shader{shader} {
    shape = __KML::defaultShape;
    shader = __KML::defaultShader;
}

Surface::Surface(std::string texture) {
    SetTexture(texture);
    shape = __KML::defaultShape;
    shader = __KML::defaultShader;
}

Surface::Surface(Vec4f transform) : pos{transform.x, transform.y}, scale{transform.z, transform.w} {
    shape = __KML::defaultShape;
    shader = __KML::defaultShader;
}

Surface::Surface(std::string texture, Vec4f transform) : pos{transform.x, transform.y}, scale{transform.z, transform.w} {
    SetTexture(texture);
    shape = __KML::defaultShape;
    shader = __KML::defaultShader;
}

Surface::Surface(std::string texture, Vec4f transform, Vec2f anchor) 
: pos{transform.x, transform.y}, scale{transform.z, transform.w} 
{
    this->anchor = anchor;
    SetTexture(texture);
    shape = __KML::defaultShape;
    shader = __KML::defaultShader;
}

Surface::Surface(std::string texture, Vec4f transform, Vec2f anchor, float rotation) 
: pos{transform.x, transform.y}, scale{transform.z, transform.w}
{
    this->anchor = anchor;
    this->rotation = rotation;
    SetTexture(texture);
    shape = __KML::defaultShape;
    shader = __KML::defaultShader;
}

void Surface::Draw() {
    assert(shape);
    assert(shader);

    glm::mat4 view = glm::mat4{1.0f}, model = glm::mat4{1.0f}, 
        proj = glm::ortho(0.0f, __KML::LOG_SCREEN_WIDTH, 0.0f, __KML::LOG_SCREEN_HEIGHT, -1.0f, 1.0f);

    model = glm::translate(model, glm::vec3{pos.x, pos.y, 0});
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0,0,1));

    //anchor
    model = glm::translate(model, glm::vec3((0.5f - anchor.x) * scale.x, (0.5f - anchor.y) * scale.y, 0));

    model = glm::scale(model, glm::vec3(scale.x, scale.y, 1));

    Vec4f finalColor = HSVtoRGBA(color);
    finalColor.w = (float)(100 - Clamp<int>(transparency, 0, 100)) / 100.0f;

    glUseProgram(KML::GetShaderID(shader));

    if(camera != nullptr) {
        view = glm::translate(glm::mat4(1.0f), glm::vec3(-camera->pos.x, -camera->pos.y, 0.0f));

        if(camera->rotation != 0.0f) view = glm::rotate(view, glm::radians(camera->rotation), glm::vec3(0,0,1));
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    glUniform1i(KML::GetShaderUniformL(shader, "uTex"), 0);
    glUniform1i(KML::GetShaderUniformL(shader, "useTex"), tex > 0 ? 1 : 0);
    glUniformMatrix4fv(KML::GetShaderUniformL(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(KML::GetShaderUniformL(shader, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(KML::GetShaderUniformL(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniform4f(KML::GetShaderUniformL(shader, "tint"), finalColor.x, finalColor.y, finalColor.z, finalColor.w);
    
    shape->Use();
}

void Surface::SetTexture(std::string texture) {
    this->tex = __KML::Texture::get(texture);
}

void Surface::SetColor_RGB(int R, int G, int B) {
    color = RGBAtoHSV({(float)R, (float)G, (float)B});
}

void Surface::SetColor_HSV(int H, int S, int V) {
    color = HSV_v3f(H, S, V);
}

Camera::Camera() {
    pos.x = 0.0f;
    pos.y = 0.0f;
    size.x = __KML::LOG_SCREEN_WIDTH;
    size.y = __KML::LOG_SCREEN_HEIGHT;
    rotation = 0.0f;
}

Camera::Camera(Vec2f pos, Vec2f scale, float rotation) {
    this->pos = pos;
    this->size = scale;
    this->rotation = rotation;
}