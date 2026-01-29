#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>

#include <vector>
#include <unordered_map>
#include <string>
#include <cstring> // memset
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

struct Glyph {
    float u0, v0, u1, v1; // UV coords in atlas
    int width, height;     // glyph bitmap size
    int xoff, yoff;        // offset relative to baseline
    int advance;           // horizontal advance
};

struct FontAtlas {
    unsigned char* texture;             // atlas bitmap RGBA
    int width, height;                  // atlas texture size
    int channels = 4;
    float scale;                        // scale for font_size
    std::unordered_map<char, Glyph> glyphs;
};

FontAtlas generateFontAtlas(const unsigned char* font_buffer, float font_size, const char* charset = nullptr) {
    FontAtlas atlas = {};
    stbtt_fontinfo font;
    assert(stbtt_InitFont(&font, font_buffer, 0));

    atlas.scale = stbtt_ScaleForPixelHeight(&font, font_size);

    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&font, &ascent, &descent, &lineGap);

    // Define atlas size
    atlas.width = 1024;
    atlas.height = 1024;
    atlas.texture = (unsigned char*)calloc(atlas.width * atlas.height * atlas.channels, 1);

    int cursor_x = 0;
    int cursor_y = 0;
    int row_height = 0;

    if (!charset) charset = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

    for (int i = 0; charset[i]; i++) {
        char c = charset[i];

        int glyph_w, glyph_h, xoff, yoff;
        unsigned char* bmp = stbtt_GetCodepointBitmap(&font, 0, atlas.scale, c, &glyph_w, &glyph_h, &xoff, &yoff);

        // New row if glyph doesn't fit
        if (cursor_x + glyph_w >= atlas.width) {
            cursor_x = 0;
            cursor_y += row_height;
            row_height = 0;
        }

        // Copy glyph bitmap to atlas
        for (int y = 0; y < glyph_h; y++) {
            for (int x = 0; x < glyph_w; x++) {
                int dst_x = cursor_x + x;
                int dst_y = cursor_y + y;
                int idx = (dst_y * atlas.width + dst_x) * 4;
                atlas.texture[idx + 0] = 255;             // R
                atlas.texture[idx + 1] = 255;             // G
                atlas.texture[idx + 2] = 255;             // B
                atlas.texture[idx + 3] = bmp[y * glyph_w + x]; // A
            }
        }

        Glyph g;
        g.width = glyph_w;
        g.height = glyph_h;
        g.xoff = xoff;
        g.yoff = yoff;
        g.advance = 0; // will set below
        g.u0 = float(cursor_x) / atlas.width;
        g.v0 = float(cursor_y) / atlas.height;
        g.u1 = float(cursor_x + glyph_w) / atlas.width;
        g.v1 = float(cursor_y + glyph_h) / atlas.height;

        int adv, lsb;
        stbtt_GetCodepointHMetrics(&font, c, &adv, &lsb);
        g.advance = int(adv * atlas.scale);

        atlas.glyphs[c] = g;

        cursor_x += glyph_w + 1; // 1 px padding
        if (glyph_h > row_height) row_height = glyph_h;

        stbtt_FreeBitmap(bmp, NULL);
    }

    return atlas;
}


// Generate a texture from a string using a TTF buffer
// font_buffer: pointer to loaded TTF data
// font_size: desired font size in pixels
// text: the string to render
// channels: 1 (grayscale) or 4 (RGBA)

// Renders a string into a single RGBA texture using stb_truetype
FontBitmap generateTextTextureAtlas(const unsigned char* font_buffer, const char* text, float font_size) {
    FontBitmap tex = {0};
    tex.channels = 4;

    stbtt_fontinfo font;
    if (!stbtt_InitFont(&font, font_buffer, 0)) {
        fprintf(stderr, "Failed to initialize font!\n");
        return tex;
    }

    float scale = stbtt_ScaleForPixelHeight(&font, font_size);

    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&font, &ascent, &descent, &lineGap);

    int baseline = (int)(ascent * scale);

    // Estimate texture width and height
    int tex_width = 1024; // max width of texture
    int cursor_x = 0, cursor_y = 0;
    int row_height = 0;

    // First pass: compute texture height
    for (int i = 0; i < strlen(text); i++) {
        int advance, lsb, w, h, xoff, yoff;
        char c = text[i];

        stbtt_GetCodepointHMetrics(&font, c, &advance, &lsb);
        stbtt_GetCodepointBitmapBox(&font, c, scale, scale, &xoff, &yoff, &w, &h);

        if (cursor_x + w >= tex_width) {
            cursor_x = 0;
            cursor_y += row_height;
            row_height = 0;
        }

        cursor_x += w + 1;
        if (h > row_height) row_height = h;
    }

    int tex_height = cursor_y + row_height;
    tex.width = tex_width;
    tex.height = tex_height;

    tex.data = (unsigned char*)calloc(tex.width * tex.height * tex.channels, 1);
    if (!tex.data) {
        fprintf(stderr, "Failed to allocate texture memory!\n");
        return tex;
    }

    // Second pass: render each glyph
    cursor_x = 0;
    cursor_y = 0;
    row_height = 0;

    for (int i = 0; i < strlen(text); i++) {
        char c = text[i];

        int glyph_w, glyph_h, xoff, yoff;
        unsigned char* glyph_bitmap = stbtt_GetCodepointBitmap(&font, 0, scale, c, &glyph_w, &glyph_h, &xoff, &yoff);

        if (cursor_x + glyph_w >= tex_width) {
            cursor_x = 0;
            cursor_y += row_height;
            row_height = 0;
        }

        for (int y = 0; y < glyph_h; y++) {
            for (int x = 0; x < glyph_w; x++) {
                int dst_x = cursor_x + x + xoff;
                int dst_y = tex.height - (cursor_y + y + yoff + baseline) - 1;

                if (dst_x < 0 || dst_x >= tex.width || dst_y < 0 || dst_y >= tex.height) continue;

                unsigned char pixel = glyph_bitmap[y * glyph_w + x];
                int idx = (dst_y * tex.width + dst_x) * 4;
                tex.data[idx + 0] = 255;
                tex.data[idx + 1] = 255;
                tex.data[idx + 2] = 255;
                tex.data[idx + 3] = pixel;
            }
        }

        int advance, lsb;
        stbtt_GetCodepointHMetrics(&font, c, &advance, &lsb);
        cursor_x += (int)(advance * scale);
        if (i < strlen(text) - 1) {
            cursor_x += (int)(stbtt_GetCodepointKernAdvance(&font, c, text[i+1]) * scale);
        }

        if (glyph_h > row_height) row_height = glyph_h;

        stbtt_FreeBitmap(glyph_bitmap, NULL);
    }

    return tex;
}

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

    FontBitmap bmp = generateTextTextureAtlas(buffer, text, 48.0f);

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