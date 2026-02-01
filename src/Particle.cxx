#include "KML/Particle.h"
#include "KML/Utils.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fmt/core.h>

#include "__KML/graphics.h"

KML::Particle KML::GenRandParticle(double minT, double maxT, Vec2f origin, Vec2f spread) {
    Particle p;
    // Posición inicial dentro de un área de emisión
    p.pos = origin + Vec2f{
        KML::RandFloat(0.0f, spread.x),
        KML::RandFloat(0.0f, spread.y)
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
                
    fmt::println("Particula: pos({},{}), vel({},{}), acc({},{}), size({}), color({},{},{}), time({})", p.pos.x, p.pos.y, p.vel.x, p.vel.y, p.acc.x, p.acc.y, p.size, p.color.x, p.color.y, p.color.z, p.time);                            
    return p;
}


KML::ParticleGroup::ParticleGroup(Shader* s) {
    shader = (s ? s : __KML::defaultShader);
    shape = __KML::defaultShape;
}

void KML::ParticleGroup::Generate(int count, double minT, double maxT, Vec2f o, Vec2f s) {
    for(int i = 0; i < count; i++) {
        particles.push_back(GenRandParticle(2.0, 6.0, o, s));
    }
}

void KML::ParticleGroup::Draw(double dt) {
    assert(shader);
    assert(shape);

    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
        [](const Particle& p){ return p.time <= 0.0f; }), particles.end()
    );

    for(Particle& p : particles) {
        p.vel += p.acc * (float)dt;
        p.pos += p.vel * (float)dt;
        p.time -= (float)dt;

        glm::mat4 view = glm::mat4{1.0f}, model = glm::mat4{1.0f}, 
        proj = glm::ortho(0.0f, __KML::LOG_SCREEN_WIDTH, 0.0f, __KML::LOG_SCREEN_HEIGHT, -1.0f, 1.0f);

        model = glm::translate(model, glm::vec3{p.pos.x, p.pos.y, 0});
        model = glm::scale(model, glm::vec3(p.size, p.size, 1));

        glUseProgram(KML::GetShaderID(shader));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);

        glUniform1i(KML::GetShaderUniformL(shader, "uTex"), 0);
        glUniform1i(KML::GetShaderUniformL(shader, "useTex"), tex > 0 ? 1 : 0);
        glUniformMatrix4fv(KML::GetShaderUniformL(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(KML::GetShaderUniformL(shader, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(KML::GetShaderUniformL(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniform4f(KML::GetShaderUniformL(shader, "tint"), p.color.x, p.color.y, p.color.z, 1.0f);
        
        shape->Use();
    }    
}