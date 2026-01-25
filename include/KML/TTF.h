#ifndef KML_TTF_H
#define KML_TTF_H

namespace KML {
    void LoadFont(const char* file);
    void UnloadFont(const char* file);
    unsigned int GetBitmap(const char* file, const char* text);
}

#endif