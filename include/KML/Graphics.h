#ifndef KML_GRAPHICS_H
#define KML_GRAPHICS_H

#include "ABI.h"
#include "Texture.h"
#include "Shader.h"

/**
 * @file        Graphics.h
 * @brief       Handles shaders and textures
 */

namespace KML {
    struct Vec2f;
    struct Vec3f;
    struct Vec4f;
    struct Framebuffer;

    /**
     * @brief       Creates a new framebuffer in GPU memory
     * @param[in]   width Logical screeen width
     * @param[in]   height Logical screen height
     * @returns     @ref Framebuffer      
     */
    KML_API Framebuffer* CreateFramebuffer(int width, int height);


    /**
     * @brief      Binds a framebuffer, where all next drawing operations will be performed
     *
     * @param      framebuffer  @ref Framebuffer
     * @param[in]  width            Screen width
     * @param[in]  height            Screeen height
     * @param[in]  cc          Clear color
     */
    KML_API void BindFramebuffer(Framebuffer* framebuffer, Vec3f cc);

    /**
     * @brief      Draws a framebuffer to the backbuffer
     *
     * @param      framebuffer  @ref Framebuffer
     * @param      shader       @ref Shader
     */
    KML_API void DrawFramebuffer(Framebuffer* framebuffer, Shader* shader);

    KML_API void DeleteFramebuffer(Framebuffer* framebuffer); 
}

#endif
