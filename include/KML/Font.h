#ifndef KML_FONT_H
#define KML_FONT_H

#include "ABI.h"
#include "Vector.h"
#include "Graphics.h"
#include "Surface.h"

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

    class KML_API Text : public Surface {
      public:
        std::string text;
        std::string font;
        Shader* shader = nullptr;
        Camera* camera = nullptr;
        Text(std::string key);
        void Draw();
    };
}

#endif