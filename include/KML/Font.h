#ifndef KML_FONT_H
#define KML_FONT_H

#include "ABI.h"
#include "Vector.h"
#include "Graphics.h"

#include <string>

/**
 * @file        Font.h
 * @brief       Font loading and rendering
 */

namespace KML {
    /**
     * @brief      Loads a font.
     *
     * @param[in]  file        TrueType font file
     * @param[in]  resolution  Pixel size of each glyph
     *
     * @return     Font key name formatted "<file>:<resolution>", use this to reference a rendered font
     */
    KML_API std::string LoadFont(const char* file, int resolution);

    /**
     * @brief      Frees all resources allocated to a font
     *
     * @param[in]  key  Font key name formatted "<file>:<resolution>"
     */
    KML_API void UnloadFont(std::string key);
    KML_API void RenderText(std::string font, Shader* shader, std::string text, float x, float y, float scale, Vec3f color);
}

#endif