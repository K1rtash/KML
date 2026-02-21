#ifndef KML_FRAMEBUFFER_H
#define KML_FRAMEBUFFER_H

#include "ABI.h"
#include "Shader.h"

/**
 * @file        Framebuffer.h
 * @brief       Handles framebuffers
 */

namespace KML {
    struct Vec3f;
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
