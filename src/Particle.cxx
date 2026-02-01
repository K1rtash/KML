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

        if(camera != nullptr) {
            view = glm::translate(glm::mat4(1.0f), glm::vec3(-camera->pos.x, -camera->pos.y, 0.0f));

            if(camera->rotation != 0.0f) view = glm::rotate(view, glm::radians(camera->rotation), glm::vec3(0,0,1));
        }

        glUniformMatrix4fv(KML::GetShaderUniformL(shader, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(KML::GetShaderUniformL(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(KML::GetShaderUniformL(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));

        shape->Use();
    }    
}