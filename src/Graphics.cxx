#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "KML/Graphics.h"

#include "__KML/graphics.h"

GLuint __KML::Shader::compile_src(GLenum type, const char* src, GLuint program) {
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    
    int status = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if(!status) {
        char buffer[1024];
        glGetShaderInfoLog(id, 1024, nullptr, buffer);
        std::cout << "ERROR:SHADER_COMPILATION: " << buffer << "\nSOURCE =>\n" << src << std::endl;
        return 0;
    } 
    glAttachShader(program, id);
    return id;
}

__KML::Shader::Shader __KML::Shader::create_program(const char* v_src, const char* f_src) {
    Shader program;
    program.id = glCreateProgram();

    GLuint vert = __KML::Shader::compile_src(GL_VERTEX_SHADER, v_src, program.id);
    GLuint frag = __KML::Shader::compile_src(GL_FRAGMENT_SHADER, f_src, program.id);

    glLinkProgram(program.id);
    glDetachShader(program.id, vert);
    glDetachShader(program.id, frag);

    int status = 0;
    glGetProgramiv(program.id, GL_LINK_STATUS, &status);
    if(!status) {
        char buffer[1024];
        glGetProgramInfoLog(program.id, sizeof(buffer), nullptr, buffer);
        std::cout << "ERROR:PROGRAM_LINK: " << buffer << "\n";
    } 
    glDeleteShader(vert);
    glDeleteShader(frag);

    glUseProgram(program.id);

    map_shader_uniforms(&program);
    return program;
}

void __KML::Shader::map_shader_uniforms(Shader* shader) {
    int u_count;
    glGetProgramiv(shader->id, GL_ACTIVE_UNIFORMS, &u_count);

    for (int i = 0; i < u_count; i++) {
        char name[256];
        GLsizei length;
        GLint size;
        GLenum type;

        glGetActiveUniform(shader->id, i, sizeof(name), &length, &size, &type, name);

        GLint location = glGetUniformLocation(shader->id, name);

        shader->uniforms[std::string(name)] = location;
    }
}

GLuint __KML::Shader::get_uniform_loc(Shader* shader, std::string name) {
    auto i = shader->uniforms.find(name);
    if(i == shader->uniforms.end()) {
        shader->uniforms[name] = -1;
        printf("Accesed unmaped uniform on shader id %d: %s", shader->id, name.c_str());
        return -1;
    }
    return i->second;
}