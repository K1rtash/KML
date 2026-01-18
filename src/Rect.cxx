#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <malloc.h>
#include <vector>

#include "__KML/graphics.h"
#include "KML/Surface.h"
#include "KML/Layers.h"

KML::Shader* program;
KML::Shader* program_noTex;

GLuint VAO, VBO, EBO;

float vertices[] = {
    0.5f,  0.5f, 0.0f,     1.0f, 1.0f,    // top right
    0.5f, -0.5f, 0.0f,     1.0f, 0.0f,    // bottom right
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,    // bottom left
    -0.5f,  0.5f, 0.0f,     0.0f, 1.0f     // top left 
};

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

void kml__gen_buffers();

void kml__tempPC() {
    const char* vertex_src = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aUV;\n"
        "out vec2 uvCoords;\n"
        "uniform mat4 model;\n"
        "uniform mat4 proj;\n"
        "void main() {\n"
        "uvCoords = aUV;\n"
        "gl_Position = proj * model * vec4(aPos, 1.0);\n"
        "}";

    const char* fragment_src = 
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec2 uvCoords;\n"
        "uniform sampler2D uTex;\n"
        "uniform vec4 tint;\n"
        "void main()\n"
        "{\n"
        "vec4 texColor = texture(uTex, uvCoords);\n"
        "texColor *= tint;\n"
        "if(texColor.a < 0.1) discard;\n"
        "FragColor = vec4(texColor.rgb, texColor.a * tint.w) * tint;\n"
        "}";

    const char* fragment_noTex_src = 
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec4 tint;\n"
        "void main()\n"
        "{\n"
        "FragColor = tint;\n"
        "}";

    program = __KML::create_program_from_src(vertex_src, fragment_src);
    program_noTex = __KML::create_program_from_src(vertex_src, fragment_noTex_src);
    kml__gen_buffers();
}

void kml__gen_buffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void drawVerticesRect(glm::mat4& model, KML::Vec4f color, KML::Shader* shader) {
    glm::mat4 projection = glm::ortho(0.0f, LOG_SCREEN_WIDTH, 0.0f, LOG_SCREEN_HEIGHT, -1.0f, 1.0f);
    glUniformMatrix4fv(KML::GetShaderUniformL(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(KML::GetShaderUniformL(shader, "proj"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform4f(KML::GetShaderUniformL(shader, "tint"), color.x, color.y, color.z, color.w);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glUseProgram(0);
    glBindVertexArray(0);
}

void __KML::draw_rect(glm::mat4& model, KML::Vec4f color, KML::Shader* shader, unsigned int tex) {
    KML::Shader* actP = (shader == nullptr) ? program : shader;

    glUseProgram(__KML::get_program_id(actP));
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    glUniform1i(KML::GetShaderUniformL(actP, "uTex"), 0);

    drawVerticesRect(model, color, actP);
}

void __KML::draw_rect(glm::mat4& model, KML::Vec4f color, KML::Shader* shader) {
    KML::Shader* actP = (shader == nullptr) ? program_noTex : shader;
    glUseProgram(__KML::get_program_id(actP));
    drawVerticesRect(model, color, actP);
}

/*void __KML::draw_rect(glm::mat4& model, KML::Vec4f color, unsigned int tex, KML::Shader* shader) {
    KML::Shader* activeProgram = (shader != nullptr) ? shader : program_noTex;

    if(shader == nullptr)
        program = (tex > 0) ? program : program_noTex;
    else program = shader;

    if(program == nullptr) throw std::runtime_error("PROGRAM IS NULLLPTR");

    glUseProgram(__KML::get_program_id(program));
    glBindVertexArray(VAO);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    glm::mat4 projection = glm::ortho(0.0f, LOG_SCREEN_WIDTH, 0.0f, LOG_SCREEN_HEIGHT, -1.0f, 1.0f);
    
    glUniform1i(KML::GetShaderUniformL(program, "uTex"), 0);
    glUniformMatrix4fv(KML::GetShaderUniformL(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(KML::GetShaderUniformL(program, "proj"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform4f(KML::GetShaderUniformL(program, "tint"), color.x, color.y, color.z, color.w);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}*/

std::vector<KML::Layer*>layers;

KML::Layer::Layer(int maxSurfaces) {
    max = maxSurfaces;
    layers.push_back(this);
}

KML::Layer::Layer() {
    max = 999999;
    layers.push_back(this);
}

void KML::Layer::Draw() {
    int n = 0;
    for(auto& i : srf) {
        if(n >= max) break;
        if(i != nullptr) i->Draw();
        n++;
    }
} 

void KML::DrawLayers(int maxLayers) {
    int n = 0;
    for(auto& layer : layers) {
        if(n >= maxLayers && maxLayers > 0) break;
        if(layer != nullptr) layer->Draw();
        n++;
    }
}

void KML::Layer::Add(Surface& __s) {
    srf.push_back(&__s);
}