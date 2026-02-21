#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "KML/Graphics.h"

struct KML::Texture {
    GLuint id = 0;
    int width = 0, height = 0;
};

KML::Texture* KML::CreateTexture(const char* file, KML_TEXTURE_WRAP_STYLE ws) {
    int w, h, cc;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(file, &w, &h, &cc, 0);
    
    #ifdef KML_PRINT_ERRORS
        if(!bytes) printf("FAILED (bytes): %s\n", file);
        if(w == 0 || h == 0) printf("FAILED (w/h): %s\n", file);
        if(!cc) printf("FAILED (cc): %s\n", file);
    #endif

    Texture* tex = new Texture;
    if(!tex || !bytes || !w || !h || !cc) return nullptr;

    glGenTextures(1, &(tex->id));
    glBindTexture(GL_TEXTURE_2D, tex->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    if(ws == KML_TEXTURE_WS_REPEAT) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    } else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    GLenum format;
    if (cc >= 4) format = GL_RGBA;
    else switch (cc) {
        case 3: format = GL_RGB; break;
        case 2: format = GL_RG; break;
        case 1: format = GL_RED; break;
        default: return nullptr;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, format, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(bytes);
    return tex;
}

void KML::BindTexture(Texture* tex, int u) {
    if(tex && u >= 0) {
        glActiveTexture(GL_TEXTURE0 + u);
        glBindTexture(GL_TEXTURE_2D, tex->id);
    }
}

void KML::DeleteTexture(Texture* tex) {
    assert(tex);
    glDeleteTextures(1, &(tex->id));
    delete tex;
}