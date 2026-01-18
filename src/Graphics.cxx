#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "KML/Graphics.h"

#include "__KML/graphics.h"

struct KML::Shader {
    GLuint id;
    std::unordered_map<std::string, GLuint> uniforms;
    std::filesystem::path vertex;
    std::filesystem::path fragment;
};

GLuint compile_shader_src(GLenum type, const char* src, GLuint program);
GLuint create_shader_program(std::string v_src, std::string f_src);
std::unordered_map<std::string, GLuint> map_shader_uniforms(GLuint id);

KML::Shader* KML::CreateShader(std::filesystem::path vertex, std::filesystem::path fragment) {
    Shader* program = (Shader*)malloc(sizeof(Shader));
    program->id = create_shader_program(vertex.string(), fragment.string());
    program->uniforms = map_shader_uniforms(program->id);
    return program;
}

unsigned int KML::GetShaderUniformL(Shader* shader, std::string uniform) {
    if(shader == nullptr) return -1;
    auto i = shader->uniforms.find(uniform);
    if(i == shader->uniforms.end()) {
        shader->uniforms[uniform] = -1;
        std::cout << std::format("[WARNING] Accesed unmaped uniform on shader id {}: {}\n", shader->id, uniform);
        return -1;
    }
    return i->second;  
}

void KML::DeleteShader(KML::Shader* shader) {
    glDeleteProgram(shader->id);
    free(shader);
    shader = nullptr;
}

KML::Shader* __KML::create_program_from_src(std::string v_src, std::string f_src) {
    KML::Shader* program = (KML::Shader*)malloc(sizeof(KML::Shader));
    program->id = create_shader_program(v_src, f_src);
    std::cout << "created program id: " << program->id << std::endl;
    program->uniforms = map_shader_uniforms(program->id);
    return program;
}

unsigned int __KML::get_program_id(KML::Shader* shader) {
    if(shader == nullptr) return 0;
    return shader->id;
}

GLuint create_shader_program(std::string v_src, std::string f_src) {
    GLuint program = glCreateProgram();

    GLuint vert = compile_shader_src(GL_VERTEX_SHADER, v_src.c_str(), program);
    GLuint frag = compile_shader_src(GL_FRAGMENT_SHADER, f_src.c_str(), program);

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

    return program;
}

GLuint compile_shader_src(GLenum type, const char* src, GLuint program) {
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

std::unordered_map<std::string, GLuint> map_shader_uniforms(GLuint id) {
    std::unordered_map<std::string, GLuint> map;
    int u_count;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &u_count);

    for (int i = 0; i < u_count; i++) {
        char name[256];
        GLsizei length;
        GLint size;
        GLenum type;

        glGetActiveUniform(id, i, sizeof(name), &length, &size, &type, name);

        GLint location = glGetUniformLocation(id, name);

        map[std::string(name)] = location;
        std::cout << "mapped: " << name << " loc: " << location  << "id: " << id << std::endl; 
    }
    return map;
}