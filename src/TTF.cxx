#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>

#include <vector>
#include <unordered_map>
#include <string>
#include <cstring> // memset
#include <fstream>
#include <iostream>
#include <cassert>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fmt/core.h>

#include "__KML/graphics.h"
#include "KML/TTF.h"

std::unordered_map<std::string, unsigned char*> fonts;

struct FontBitmap {
    unsigned char* data; // apunta al buffer generado
    int width;
    int height;
    int channels;
};

// Generate a texture from a string using a TTF buffer
// font_buffer: pointer to loaded TTF data
// font_size: desired font size in pixels
// text: the string to render
// channels: 1 (grayscale) or 4 (RGBA)
FontBitmap generateTextTexture(const unsigned char* font_buffer, float font_size, const char* text, int channels) {
    FontBitmap tex = {0};

    stbtt_fontinfo font;
    if (!stbtt_InitFont(&font, font_buffer, 0)) {
        fprintf(stderr, "Failed to initialize font!\n");
        return tex;
    }

    float scale = stbtt_ScaleForPixelHeight(&font, font_size);

    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&font, &ascent, &descent, &lineGap);
    int baseline = (int)(ascent * scale);

    // First pass: compute total width
    int width = 0;
    for (int i = 0; i < strlen(text); i++) {
        int advance, lsb;
        stbtt_GetCodepointHMetrics(&font, text[i], &advance, &lsb);
        width += (int)(advance * scale);

        // kerning
        if (i < strlen(text) - 1) {
            width += (int)(stbtt_GetCodepointKernAdvance(&font, text[i], text[i+1]) * scale);
        }
    }

    int height = baseline - (int)(descent * scale);
    tex.width = width;
    tex.height = height;
    tex.channels = channels;
    tex.data = (unsigned char*)calloc(width * height * channels, 1);
    if (!tex.data) {
        fprintf(stderr, "Failed to allocate texture memory!\n");
        return tex;
    }

    // Second pass: render each character
    int cursor_x = 0;
    for (int i = 0; i < strlen(text); i++) {
        int glyph_w, glyph_h, xoff, yoff;
        unsigned char* glyph_bitmap = stbtt_GetCodepointBitmap(&font, 0, scale, text[i], &glyph_w, &glyph_h, &xoff, &yoff);

        for (int y = 0; y < glyph_h; y++) {
            for (int x = 0; x < glyph_w; x++) {
                int dst_x = cursor_x + x + xoff;
                int dst_y = tex.height - (baseline + yoff + y) - 1; // align baseline

                if (dst_x < 0 || dst_x >= width || dst_y < 0 || dst_y >= height) continue;

                unsigned char pixel = glyph_bitmap[y * glyph_w + x];
                if (channels == 1) {
                    tex.data[dst_y * width + dst_x] = pixel;
                } else if (channels == 4) {
                    int idx = (dst_y * width + dst_x) * 4;
                    tex.data[idx + 0] = 255;    // R
                    tex.data[idx + 1] = 255;    // G
                    tex.data[idx + 2] = 255;    // B
                    tex.data[idx + 3] = pixel;  // A
                }
            }
        }

        int advance, lsb;
        stbtt_GetCodepointHMetrics(&font, text[i], &advance, &lsb);
        cursor_x += (int)(advance * scale);
        if (i < strlen(text) - 1) {
            cursor_x += (int)(stbtt_GetCodepointKernAdvance(&font, text[i], text[i+1]) * scale);
        }

        stbtt_FreeBitmap(glyph_bitmap, NULL);
    }

    return tex;
}

// Example usage:
// 1. Load TTF into memory
// 2. Call generateTextTexture()
// 3. Use tex.data with width, height, channels



// obtener los bytes de la TTF cargada
unsigned char* __KML::getTTF(std::string name) {
    if (fonts.find(name) == fonts.end()) return NULL;
    return fonts.at(name);
}

// crea textura OpenGL a partir de un texto
unsigned int KML::GetBitmap(const char* file, const char* text) { 
    unsigned char* buffer = __KML::getTTF(file);

    if (!buffer) {
        fmt::print("[DEBUG] Set text: no entry: {}\n", file);
        return 0;
    }

    FontBitmap bmp = generateTextTexture(buffer, 48.0f, text, 4);

    unsigned int texid = __KML::Texture::loadTexToGL(bmp.data, bmp.width, bmp.height, bmp.channels);
    free(bmp.data);
    return texid;
}

// carga el TTF en memoria
void KML::LoadFont(const char* __f) {
    if (fonts.find(__f) != fonts.end()) return;

    FILE* file = fopen(__f, "rb");
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    unsigned char* font_buffer = (unsigned char*)malloc(size);
    fread(font_buffer, 1, size, file);
    fclose(file);

    fonts[__f] = font_buffer;
}

// elimina font cargada
void KML::UnloadFont(const char* __f) {
    if (fonts.find(__f) == fonts.end()) return;

    free(fonts[__f]);
    fonts.erase(__f);
}