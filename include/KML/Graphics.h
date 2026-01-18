#ifndef KML_GRAPHICS_H
#define KML_GRAPHICS_H

#include <string>
#include <filesystem>

namespace KML {
    struct Shader;
    Shader* CreateShader(std::filesystem::path vertex, std::filesystem::path fragment);
    unsigned int GetShaderUniformL(Shader* shader, std::string uniform);
    void DeleteShader(Shader* shader);

    bool LoadTexture(const char* file);
    void UnloadTexture(const char* file);
}

#endif