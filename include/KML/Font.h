#ifndef KML_FONT_H
#define KML_FONT_H

#include "ABI.h"
#include "Vector.h"
#include "Graphics.h"


namespace KML {
    KML_API void LoadFont(const char* file);
    //KML_API void UnloadFont(const char* file);
    //KML_API unsigned int GetBitmap(const char* file, const char* text);
    KML_API void RenderText(Shader* shader, std::string text, float x, float y, float scale, Vec3f color);
}

#endif