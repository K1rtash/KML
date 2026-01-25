#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>
#include <stb/stb_image.h>

#include <vector>
#include <unordered_map>
#include <string>
#include <cstring> // memset
#include <fstream>
#include <iostream>
#include <cassert>

#include "__KML/graphics.h"
#include "KML/TTF.h"

std::unordered_map<std::string, std::vector<unsigned char>> fonts;

struct FontBitmap {
    unsigned char* pixels = nullptr; // apunta al buffer generado
    int width = 0;
    int height = 0;
    int channels = 0;
};

// Devuelve un FontBitmap con los datos del texto renderizado
FontBitmap RenderTextFromTTF(const unsigned char* ttf_data, size_t ttf_size,
                             const std::string& text, float pixel_height,
                             int channels = 1)
{
    FontBitmap result;

    stbtt_fontinfo font;
    if (!stbtt_InitFont(&font, ttf_data, 0)) {
        std::cerr << "[DEBUG] Failed to init font\n";
        return result;
    }

    // Obtenemos métricas de altura
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&font, &ascent, &descent, &lineGap);
    float scale = stbtt_ScaleForPixelHeight(&font, pixel_height);


    // altura real del bitmap
    int height_top = int(ascent * scale);
    int height_bottom = int(-descent * scale);
    int bitmap_height = height_top + height_bottom;
    int baseline = height_top;

    // Calculamos el ancho total necesario
    int pen_x = 0;
    int c_prev = 0;
    int bitmap_width = 0;

    for (char c : text) {
        int advance, lsb;
        stbtt_GetCodepointHMetrics(&font, c, &advance, &lsb);
        if (c_prev) pen_x += stbtt_GetCodepointKernAdvance(&font, c_prev, c) * scale;
        c_prev = c;

        int x0, y0, x1, y1;
        stbtt_GetCodepointBitmapBox(&font, c, scale, scale, &x0, &y0, &x1, &y1);

        int w = x1 - x0;
        int h = y1 - y0;

        std::vector<unsigned char> glyph(w * h);
        stbtt_MakeCodepointBitmap(&font, glyph.data(), w, h, w, scale, scale, c);

        // ahora pegamos el glifo
        int dst_y = baseline - y1; // baseline - top of glyph
    }

    // Guardamos dimensiones
    result.width = bitmap_width;
    result.height = bitmap_height;
    result.channels = channels;

    result.pixels = new unsigned char[bitmap_width * bitmap_height * channels];
    std::memset(result.pixels, 0, bitmap_width * bitmap_height * channels);

    // Ahora dibujamos los caracteres
    pen_x = 0;
    c_prev = 0;
    for (char c : text) {
        int advance, lsb;
        stbtt_GetCodepointHMetrics(&font, c, &advance, &lsb);
        if (c_prev) pen_x += stbtt_GetCodepointKernAdvance(&font, c_prev, c) * scale;
        c_prev = c;

        int x0, y0, x1, y1;
        stbtt_GetCodepointBitmapBox(&font, c, scale, scale, &x0, &y0, &x1, &y1);

        int w = x1 - x0;
        int h = y1 - y0;
        if (w <= 0 || h <= 0) continue;

        std::vector<unsigned char> glyph(w * h);
        stbtt_MakeCodepointBitmap(&font, glyph.data(), w, h, w, scale, scale, c);

        for (int gy = 0; gy < h; ++gy) {
            for (int gx = 0; gx < w; ++gx) {
                int src = gx + gy * w;
                int dst_x = pen_x + gx + x0;
                int dst_y = bitmap_height - (baseline + gy + y0); // flip vertical
                int dst = (dst_x + dst_y * bitmap_width) * channels;

                if (dst + channels > bitmap_width * bitmap_height * channels) continue;

                if (channels == 1) result.pixels[dst] = glyph[src];
                else if (channels == 4) {
                    result.pixels[dst + 0] = 255;
                    result.pixels[dst + 1] = 255;
                    result.pixels[dst + 2] = 255;
                    result.pixels[dst + 3] = glyph[src];
                }
            }
        }

        pen_x += int(advance * scale);
    }

    return result;
}


// obtener los bytes de la TTF cargada
std::vector<unsigned char> __KML::getTTF(std::string name) {
    if (fonts.find(name) == fonts.end()) return std::vector<unsigned char>();
    return fonts.at(name);
}

// crea textura OpenGL a partir de un texto
unsigned int KML::GetBitmap(const char* file, const char* text) { 
    std::vector<unsigned char> buffer = __KML::getTTF(file);
    if (buffer.empty()) {
        std::cerr << "[DEBUG] Set text: no entry: " << file << "\n";
        return 0;
    }

    std::string txt = text;
    FontBitmap bmp = RenderTextFromTTF(buffer.data(), buffer.size(), txt, 48.0f, 4);

    // bmp.pixels ahora contiene los bytes del bitmap RGBA
    stbi_set_flip_vertically_on_load(true);
    unsigned int texid = __KML::Texture::loadTexToGL(bmp.pixels, bmp.width, bmp.height, bmp.channels);

    delete[] bmp.pixels;
    return texid;
}

// carga el TTF en memoria
void KML::LoadFont(const char* __f) {
    std::ifstream file(__f, std::ios::binary | std::ios::ate);
    size_t size = file.tellg();
    std::vector<unsigned char> buffer(size);
    file.seekg(0);
    file.read((char*)buffer.data(), size);
    file.close();

    if (fonts.find(__f) != fonts.end()) return;
    fonts[__f] = buffer;
}

// elimina font cargada
void KML::UnloadFont(const char* __f) {
    fonts.erase(__f);
}