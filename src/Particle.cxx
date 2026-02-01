#include "KML/Particle.h"
#include "KML/Utils.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "__KML/graphics.h"


KML::ParticleGroup::ParticleGroup(Shader* s) {
    shader = (s ? s : __KML::defaultShader);
    shape = __KML::defaultShape;
}

void KML::ParticleGroup::Generate(int count, void* ptr) {
    for(int i = 0; i < count; i++) {
        Particle p;
        CustomGenerator(p, ptr);
        _particles.push_back(p);
    }
}

void KML::ParticleGroup::CustomGenerator(Particle& p, void* ptr) {
    // Posición inicial dentro de un área de emisión
    KML::Vec2f o{400.0f, 300.0f}, s{100.0f, 200.0f};
    float minT = 6.0, maxT = 12.0;
    p.pos = o + Vec2f{
        KML::RandFloat(0.0f, s.x),
        KML::RandFloat(0.0f, s.y)
    };

    // Velocidad inicial moderada
    p.vel = KML::Vec2f{
        KML::RandFloat(-5.0f, 5.0f),   // permite moverse a la izquierda o derecha
        KML::RandFloat(-5.0f, 5.0f)   // dirección hacia arriba (como humo/partículas flotantes)
    };

    // Aceleración pequeña para suavizar el movimiento
    p.acc = KML::Vec2f{
        KML::RandFloat(-1.0f, 1.0f),
        KML::RandFloat(-1.0f, 1.0f)    // ligera gravedad hacia abajo
    };

    p.size = KML::RandFloat(10.0f, 20.0f);   // tamaño moderado
    p.maxTime = KML::RandFloat((float)minT, (float)maxT);    // vida inicial más larga     
    p.color = KML::Vec3f {KML::RandFloat(0.0f, 1.0f), KML::RandFloat(0.0f, 1.0f), KML::RandFloat(0.0f, 1.0f)};    
    p.time = p.maxTime;  
}

void KML::ParticleGroup::Draw(double dt) {
    assert(shader);
    assert(shape);

    _deltaTime = (float)dt;

    _particles.erase(
        std::remove_if(_particles.begin(), _particles.end(),
        [](const Particle& p){ return p.time <= 0.0f; }), _particles.end()
    );

    for(Particle& p : _particles) {
        glUseProgram(KML::GetShaderID(shader));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);

        CustomRenderer(p);

        glm::mat4 view = glm::mat4{1.0f}, model = glm::mat4{1.0f}, 
        proj = glm::ortho(0.0f, __KML::LOG_SCREEN_WIDTH, 0.0f, __KML::LOG_SCREEN_HEIGHT, -1.0f, 1.0f);

        model = glm::translate(model, glm::vec3{p.pos.x, p.pos.y, 0});
        model = glm::scale(model, glm::vec3(p.size, p.size, 1));

        glUniformMatrix4fv(KML::GetShaderUniformL(shader, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(KML::GetShaderUniformL(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(KML::GetShaderUniformL(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));

        shape->Use();
    }    
}

void KML::ParticleGroup::CustomRenderer(Particle& p) {
    p.vel += p.acc * _deltaTime;
    p.pos += p.vel * _deltaTime;
    p.time -= _deltaTime;

    glUniform4f(KML::GetShaderUniformL(shader, "tint"), p.color.x, p.color.y, p.color.z, 1.0f);
    glUniform1i(KML::GetShaderUniformL(shader, "uTex"), 0);
    glUniform1i(KML::GetShaderUniformL(shader, "useTex"), tex > 0 ? 1 : 0);
}