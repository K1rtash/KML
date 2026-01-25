#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "KML/Graphics.h"
#include "__KML/graphics.h"

std::unordered_map<std::string, unsigned int>textures;

KML::Texture KML::LoadTexture(const char* file) {
    if(textures.find(file) != textures.end()) return __KML::Texture::get(file);
    unsigned int id = __KML::Texture::load(file);
    assert(id != 0);
    textures.insert({file, id});
    return id;
}

void KML::UnloadTexture(const char* file) {
    GLuint id = __KML::Texture::get(file);
    glDeleteTextures(1, &id);
    textures.erase(file);
}

unsigned int __KML::Texture::load(const char* file) {
    stbi_set_flip_vertically_on_load(true);

    int w, h, cc;
    unsigned char* bytes = stbi_load(file, &w, &h, &cc, 0);
    assert(bytes);
    if (!bytes) return 0;

    GLuint id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLenum format;
    if (cc >= 4) format = GL_RGBA;
    else switch (cc) {
        case 3: format = GL_RGB; break;
        case 2: format = GL_RG; break;
        case 1: format = GL_RED; break;
        default: return false;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, format, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    return id;
}

unsigned int __KML::Texture::get(std::string name) {
    if(textures.find(name) == textures.end()) return 0;
    return textures.at(name);
}