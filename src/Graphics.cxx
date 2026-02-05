#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cassert>

#include "KML/Graphics.h"
#include "KML/Vector.h"
#include "KML/Utils.h"

#include "__KML/graphics.h"

struct KML::Shader {
    GLuint id;
    std::unordered_map<std::string, GLuint> uniforms;
    std::filesystem::path vertex;
    std::filesystem::path fragment;
    bool reloadable;
};

GLuint compile_shader_src(GLenum type, const char* src, GLuint program);
GLuint create_shader_program(const char* v_src, const char* f_src);
void map_shader_uniforms(KML::Shader* shader);

KML::Shader* KML::CreateShader(std::filesystem::path vertex, std::filesystem::path fragment) {
    Shader* program = new Shader;
    program->id = create_shader_program(KML::ReadFile(vertex).c_str(), KML::ReadFile(fragment).c_str());
    assert(program);
    if(program->id == 0) {
        delete program;
        return nullptr;
    }
    program->vertex = vertex;
    program->fragment = fragment;
    program->reloadable = true;
    map_shader_uniforms(program);
    return program;
}

KML::Shader* KML::CreateShaderFS(const char* vertex, const char* fragment) {
    Shader* program = new Shader;
    program->id = create_shader_program(vertex, fragment);
    assert(program);
    if(program->id == 0) {
        delete program;
        return nullptr;
    }
    program->reloadable = false;
    map_shader_uniforms(program);
    return program;
}

int KML::GetShaderUniformL(Shader* shader, const char* uniform) {
    if(shader == nullptr || uniform == nullptr) return -1;
    auto i = shader->uniforms.find(uniform);
    if(i == shader->uniforms.end()) {
        shader->uniforms[uniform] = -1;
        #ifdef KML_PRINT_ERRORS
            printf("[KML] Accesed unmaped uniform on shader id %d: %s\n", shader->id, uniform);
        #endif
        return -1;
    }
    return i->second;  
}

void KML::ReloadShader(Shader* shader) {
    if(!shader || shader->reloadable == false) return;
    glDeleteProgram(shader->id);
    shader->id = create_shader_program(KML::ReadFile(shader->vertex).c_str(), KML::ReadFile(shader->fragment).c_str());
    if(shader->id == 0) {
        DeleteShader(shader);
        return;
    }
    map_shader_uniforms(shader);
}

void KML::DeleteShader(Shader*& shader) {
    glDeleteProgram(shader->id);
    delete shader;
    shader = nullptr;
}

void KML::SetUniform_1f(const char* u, Shader* s, float v0) {
    int loc = GetShaderUniformL(s, u);
    if (loc >= 0) { 
        glUseProgram(s->id);
        glUniform1f(loc, v0);
    }
}
void KML::SetUniform_1i(const char* u, Shader* s, int v0) {
    int loc = GetShaderUniformL(s, u);
    if (loc >= 0) {
        glUseProgram(s->id);
        glUniform1i(loc, v0);
    }
}
void KML::SetUniform_2f(const char* u, Shader* s, float v0, float v1) {
    int loc = GetShaderUniformL(s, u);
    if (loc >= 0) {
        glUseProgram(s->id);
        glUniform2f(loc, v0, v1);
    }
}
void KML::SetUniform_3f(const char* u, Shader* s, float v0, float v1, float v2) {
    int loc = GetShaderUniformL(s, u);
    if (loc >= 0) {
        glUseProgram(s->id);
        glUniform3f(loc, v0, v1, v2);
    }
}
void KML::SetUniform_4f(const char* u, Shader* s, float v0, float v1, float v2, float v3) {
    int loc = GetShaderUniformL(s, u);
    if (loc >= 0) {
        glUseProgram(s->id);
        glUniform4f(loc, v0, v1, v2, v3);
    }
}
void KML::SetUniform_2fv(const char* u, Shader* s, const Vec2f& v) {
    SetUniform_2f(u, s, v.x, v.y);
}
void KML::SetUniform_3fv(const char* u, Shader* s, const Vec3f& v) {
    SetUniform_3f(u, s, v.x, v.y, v.z);

}
void KML::SetUniform_4fv(const char* u, Shader* s, const Vec4f& v) {
    SetUniform_4f(u, s, v.x, v.y, v.z, v.w);
}

unsigned int KML::GetShaderID(Shader* shader) {
    if(shader == nullptr) return 0;
    return shader->id;
}

GLuint create_shader_program(const char* v_src, const char* f_src) {
    GLuint program = glCreateProgram();

    GLuint vert = compile_shader_src(GL_VERTEX_SHADER, v_src, program);
    GLuint frag = compile_shader_src(GL_FRAGMENT_SHADER, f_src, program);

    glLinkProgram(program);
    glDetachShader(program, vert);
    glDetachShader(program, frag);

    int status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(!status) {
        char buffer[1024];
        glGetProgramInfoLog(program, sizeof(buffer), nullptr, buffer);
        #ifdef KML_PRINT_ERRORS
            #include <iostream>
            std::cout << "ERROR:PROGRAM_LINK: " << buffer << "\nSOURCE =>\n" << src << std::endl;
        #endif
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
        #ifdef KML_PRINT_ERRORS
            #include <iostream>
            std::cout << "ERROR:SHADER_COMPILATION: " << buffer << "\nSOURCE =>\n" << src << std::endl;
        #endif
        return 0;
    } 
    glAttachShader(program, id);
    return id;
}

void map_shader_uniforms(KML::Shader* shader) {
    assert(shader && "Shader no puede ser null");

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