#include <glad/glad.h>
#include <stb/stb_image.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "KML/Graphics.h"
#include "KML/Vector.h"
#include "__KML/graphics.h"

__KML::Shader::Shader program;

GLuint VAO, VBO, EBO;
std::unordered_map<std::string, unsigned int>textures;
int u_Tex = 0, u_Model, u_Tint, u_Proj;

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
    __KML::Shader::map_shader_uniforms(&program);

    u_Model = __KML::Shader::get_uniform_loc(&program, "model");
    u_Tint = __KML::Shader::get_uniform_loc(&program, "tint");
    u_Proj = __KML::Shader::get_uniform_loc(&program, "proj");
    u_Tex = __KML::Shader::get_uniform_loc(&program, "uTex");
    std::cout << "se ha creado el programa\n";
    kml__gen_buffers();
}

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
    GLuint program = glCreateProgram();

    GLuint vert = __KML::Shader::compile_src(GL_VERTEX_SHADER, v_src, program);
    GLuint frag = __KML::Shader::compile_src(GL_FRAGMENT_SHADER, f_src, program);

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
    Shader p;
    p.id = program;
    return p;
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

void kml__drawVertices(glm::mat4& model, unsigned int tex, glm::vec4 color) {
    glUseProgram(program.id);
    glBindVertexArray(VAO);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(u_Tex, 0);

    glm::mat4 projection = glm::ortho(0.0f, LOG_SCREEN_WIDTH, 0.0f, LOG_SCREEN_HEIGHT, -1.0f, 1.0f);
    glUniformMatrix4fv(u_Model, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(u_Proj, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform4fv(u_Tint, 1, glm::value_ptr(color));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

bool KML::LoadTexture(const char* file) {
    if(textures.find(file) != textures.end()) return false;
    stbi_set_flip_vertically_on_load(true);

    int w, h, cc;
    unsigned char* bytes = stbi_load(file, &w, &h, &cc, 0);

    GLuint id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLenum format;
    if (cc >= 4) format = GL_RGBA;
    else switch (cc) {
        case 3: format = GL_RGB; break;
        case 2: format = GL_RG; break;
        case 1: format = GL_RED; break;
        default: return false;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, format, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    textures.insert({file, id});
    return true;
}

unsigned int kml__getTex(std::string name) {
    if(textures.find(name) == textures.end()) return 0;
    return textures.at(name);
}