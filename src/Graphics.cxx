#include <glad/glad.h>
#include <stb/stb_image.h>

#include <iostream>

#include "graphics.h"

GLuint program, VAO, VBO, EBO;
int u_Tex = 0;

float vertices[] = {
        0.5f,  0.5f, 0.0f,     1.0f, 1.0f,    // top right
        0.5f, -0.5f, 0.0f,     1.0f, 0.0f,    // bottom right
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,    // bottom left
    -0.5f,  0.5f, 0.0f,     0.0f, 1.0f     // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
};

void processShader(unsigned int id, const char* src) {
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    
    int status = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if(!status) {
        char buffer[1024];
        glGetShaderInfoLog(id, 1024, nullptr, buffer);
        std::cout << "ERROR:SHADER_COMPILATION: " << buffer << "\nSOURCE =>\n" << src << std::endl;
    } 
    else glAttachShader(program, id);
}

void setup_textures() {
    u_Tex = glGetUniformLocation(program, "uTex");
    stbi_set_flip_vertically_on_load(true);
}

void shader_setup(){
    const char* vertex_src = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aUV;\n"
        "out vec2 uvCoords;\n"
        "void main() {\n"
        "uvCoords = aUV;\n"
        "gl_Position = vec4(aPos, 1.0);\n"
        "}";

    const char* fragment_src = 
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec2 uvCoords;\n"
        "uniform sampler2D uTex;\n"
        "void main()\n"
        "{\n"
        "FragColor = texture(uTex, uvCoords);\n"
        "}";

    GLuint vert = glCreateShader(GL_VERTEX_SHADER), frag = glCreateShader(GL_FRAGMENT_SHADER);
    
    program = glCreateProgram();

    processShader(vert, vertex_src);
    processShader(frag, fragment_src);

    glLinkProgram(program);
    glDetachShader(program, vert);
    glDetachShader(program, frag);

    int status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(!status) {
        char buffer[1024];
        glGetProgramInfoLog(program, sizeof(buffer), nullptr, buffer);
        std::cout << "ERROR:PROGRAM_LINK: " << buffer << "\n";
    } 
    glDeleteShader(vert);
    glDeleteShader(frag);

    glUseProgram(program);
    gen_buffers();
    setup_textures();
}

void gen_buffers() {
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

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void render() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}