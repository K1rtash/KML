#ifndef KML_GRAPHICS_H
#define KML_GRAPHICS_H

#include <string>
#include <filesystem>

namespace KML {
    struct Vec2f;
    struct Vec3f;
    struct Vec4f;
    struct Shader;

    typedef unsigned int Texture;

    Shader* CreateShader(std::filesystem::path vertex, std::filesystem::path fragment);
    Shader* CreateShader(const char* vertex, const char* fragment);
    int GetShaderUniformL(Shader* shader, const char* uniform);
    unsigned int GetShaderID(Shader* shader);
    
    void SetUniform_1f(const char* uniform, Shader* shader, float v0);
    void SetUniform_1i(const char* uniform, Shader* shader, int v0);

    void SetUniform_2f(const char* uniform, Shader* shader, float v0, float v1);
    void SetUniform_3f(const char* uniform, Shader* shader, float v0, float v1, float v2);
    void SetUniform_4f(const char* uniform, Shader* shader, float v0, float v1, float v2, float v3);

    void SetUniform_2fv(const char* uniform, Shader* shader, const Vec2f& __v);
    void SetUniform_3fv(const char* uniform, Shader* shader, const Vec3f& __v);
    void SetUniform_4fv(const char* uniform, Shader* shader, const Vec4f& __v);

    void ReloadShader(Shader* shader);
    void DeleteShader(Shader*& shader);

    Texture LoadTexture(const char* file);
    void UnloadTexture(const char* file);
}

#endif