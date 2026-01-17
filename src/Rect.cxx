#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "__KML/graphics.h"

__KML::Shader::Shader program;

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
        "FragColor = texColor;\n"
        "}";

    program = __KML::Shader::create_program(vertex_src, fragment_src);
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


void __KML::Rect::draw(glm::mat4& model, KML::Vec4f color, unsigned int tex) {
    glUseProgram(program.id);
    glBindVertexArray(VAO);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    glm::mat4 projection = glm::ortho(0.0f, LOG_SCREEN_WIDTH, 0.0f, LOG_SCREEN_HEIGHT, -1.0f, 1.0f);
    
    glUniform1i(get_uniform_loc(&program, "uTex"), 0);
    glUniformMatrix4fv(get_uniform_loc(&program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(get_uniform_loc(&program, "proj"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform4f(get_uniform_loc(&program, "tint"), color.x, color.y, color.z, color.w);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}