#ifndef KML_TEXTURE_H
#define KML_TEXTURE_H     

#include "ABI.h"

/**
 * @file    Texture.h
 * @brief   GPU texture
 */ 

#define KML_TEXTURE_WRAP_STYLE int
#define KML_TEXTURE_WS_REPEAT 0
#define KML_TEXTURE_WS_CLAMP 1

namespace KML {
    struct Texture;

     /**
     * @brief      Loads a texture from disc
     *
     * @param[in]  file  Disc file
     *
     * @return     @ref Texture
     */
    KML_API Texture* CreateTexture(const char* file, KML_TEXTURE_WRAP_STYLE wrapStyle = 0);

    /**
     * @brief      Binds a loaded texture to a texture unit, which can be passed to a shader's sampler2D uniform
     *
     * @param[in]  texture  @ref Texture
     * @param[in]  unit     Texture unit
     * 
     * Texture units are arbitary values defined by the user, their purpose is linking a GPU Texture with a channel
     */
    KML_API void BindTexture(Texture* texture, int unit);

    /**
     * @brief      Frees all allocated resources in a texture
     *
     * @param[in]  texture    @ref Texture
     */
    KML_API void DeleteTexture(Texture* texture);
}

#endif