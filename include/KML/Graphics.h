#ifndef KML_GRAPHICS_H
#define KML_GRAPHICS_H

#include "ABI.h"

#include <string>
#include <filesystem>

/**
 * @file        Graphics.h
 * @brief       Handles shaders and textures
 */

namespace KML {
    struct Vec2f;
    struct Vec3f;
    struct Vec4f;
    struct Shader;

    typedef unsigned int Texture;

    /**
     * @brief      Creates a custom shader program from disc files
     *
     * @param[in]  vertex    Vertex shader file
     * @param[in]  fragment  Fragment shader file
     *
     * @return     @ref Shader
     */
    KML_API Shader* CreateShader(std::filesystem::path vertex, std::filesystem::path fragment);

    /**
     * @brief      Creates a custom shader program from raw source
     *
     * @param[in]  vertex    Vertex shader source
     * @param[in]  fragment  Fragment shader source
     *
     * @return     @ref Shader
     */
    KML_API Shader* CreateShaderFS(const char* vertex, const char* fragment);
    
    /**
     * @brief      Retrieves the location of a uniform in a shader program
     *
     * @param      shader   @ref Shader program
     * @param[in]  uniform  Uniform name
     *
     * @return     The uniforms location
     */
    KML_API int GetShaderUniformL(Shader* shader, const char* uniform);

    /**
     * @brief      Gets the shader id.
     *
     * @param      shader  @ref Shader
     *
     * @return     Shader id
     */
    KML_API unsigned int GetShaderID(Shader* shader);

    /**
     * @brief      Binds a shader program so all subsecuent GL calls will be performed on it
     *
     * @param      shader  @ref Shader
     */
    KML_API void UseShader(Shader* shader);
    
    KML_API void SetUniform_1f(const char* uniform, Shader* shader, float v0);
    KML_API void SetUniform_1i(const char* uniform, Shader* shader, int v0);

    KML_API void SetUniform_2f(const char* uniform, Shader* shader, float v0, float v1);
    KML_API void SetUniform_3f(const char* uniform, Shader* shader, float v0, float v1, float v2);
    KML_API void SetUniform_4f(const char* uniform, Shader* shader, float v0, float v1, float v2, float v3);

    KML_API void SetUniform_2fv(const char* uniform, Shader* shader, const Vec2f& __v);
    KML_API void SetUniform_3fv(const char* uniform, Shader* shader, const Vec3f& __v);
    KML_API void SetUniform_4fv(const char* uniform, Shader* shader, const Vec4f& __v);

    /**
     * @brief      Compiles the shader's source files and maps uniform locations
     *
     * @param      shader  @ref Shader
     * 
     * @warning    The shader's source must have been retrieved from disc 
     */
    KML_API void ReloadShader(Shader* shader);

    /**
     * @brief      Frees all allocated resources in a shader program
     *
     * @param      shader  @ref Shader
     */
    KML_API void DeleteShader(Shader*& shader);

    /**
     * @brief      Loads a texture from file, will be saved as the char* you provided
     *
     * @param[in]  file  Disc file
     *
     * @return     Unique id
     */
    KML_API Texture LoadTexture(const char* file);

    /**
     * @brief      Binds a loaded texture to a texture unit, which can be passed to a shader's sampler2D uniform
     *
     * @param[in]  texture  @ref Texture
     * @param[in]  unit     Texture unit
     */
    KML_API void BindTexture(Texture texture, int unit);

    /**
     * @brief      Frees all allocated resources in a texture
     *
     * @param[in]  file  Disc file
     */
    KML_API void UnloadTexture(const char* file);
}

#endif