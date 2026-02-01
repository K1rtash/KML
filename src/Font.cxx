#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <unordered_map>
#include <iostream>
#include <string>
#include <cassert>


#include "KML/Font.h"
#include "KML/Utils.h"
#include "__KML/graphics.h"


struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

struct Font {
    std::map<char, Character> glyphs;
    int size;
};

std::unordered_map<std::string, Font> g_fonts;

GLuint VAO, VBO;
KML::Shader* text_shader = nullptr;
FT_Library ft;

void loadFontAsFace(const char* file, int px) {
	FT_Face face;
    if (FT_New_Face(ft, file, 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, px);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
	  
	Font font;
	font.size = px;

	for (unsigned char c = 0; c < 128; c++)
	{
	    // load character glyph 
	    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
	    {
	        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
	        continue;
	    }
	    // generate texture
	    unsigned int texture;
	    glGenTextures(1, &texture);
	    glBindTexture(GL_TEXTURE_2D, texture);
	    glTexImage2D(
	        GL_TEXTURE_2D,
	        0,
	        GL_RED,
	        face->glyph->bitmap.width,
	        face->glyph->bitmap.rows,
	        0,
	        GL_RED,
	        GL_UNSIGNED_BYTE,
	        face->glyph->bitmap.buffer
	    );
	    // set texture options
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    // now store character for later use
	    Character character = {
	        texture, 
	        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
	        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
	        face->glyph->advance.x
	    };
	    font.glyphs.insert(std::pair<char, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);

    g_fonts[std::string(file) + std::string(":") + std::to_string(px)] = std::move(font);
}

std::string KML::LoadFont(const char* file, int resolution) {
	std::string key = file + std::string(":") + std::to_string(resolution);
	if(g_fonts.find(key) == g_fonts.end()) loadFontAsFace(file, resolution);
	return key;
}

void KML::UnloadFont(std::string key) {
	auto it = g_fonts.find(key);
    if(it == g_fonts.end()) return;

    for(auto& [c, ch] : it->second.glyphs) {
        glDeleteTextures(1, &ch.TextureID);
    }
    g_fonts.erase(it);
}

void __KML::InitFreeType() {
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    const char* vert_src = 
	    "#version 330 core\n"
		"layout (location = 0) in vec4 vertex;\n"
		"out vec2 TexCoords;\n"
		"uniform mat4 MVP;\n"
		"void main()\n"
		"{\n"
	    "gl_Position = MVP * vec4(vertex.xy, 0.0, 1.0);\n"
	    "TexCoords = vertex.zw;\n"
		"}";

	const char* frag_src =
		"#version 330 core\n"
		"in vec2 TexCoords;\n"
		"out vec4 color;\n"
		"uniform sampler2D text;\n"
		"uniform vec3 textColor;\n"
		"void main()\n"
		"{\n"    
	    "vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
	    "color = vec4(textColor, 1.0) * sampled;\n"
		"}";

	text_shader = KML::CreateShaderFS(vert_src, frag_src);
	assert(text_shader);
	glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void __KML::QuitFreeType() {
	for(const auto& it : g_fonts) {
		for(auto& [c, ch] : it.second.glyphs) glDeleteTextures(1, &ch.TextureID);
	}
	g_fonts.clear();
	KML::DeleteShader(text_shader);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	FT_Done_FreeType(ft);
}

void renderText(auto it, std::string text, float scale) {
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    const auto& chars = it->second.glyphs;
    float cursorX = 0.0f; // posición relativa dentro del texto

    for(char c : text) {
        const Character& ch = chars.at(c);

        // Local coordinates (0,0) = start of text
        float xpos = cursorX + ch.Bearing.x * scale;
        float ypos = -(ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0, 0 },
            { xpos,     ypos,       0, 1 },
            { xpos + w, ypos,       1, 1 },
            { xpos,     ypos + h,   0, 0 },
            { xpos + w, ypos,       1, 1 },
            { xpos + w, ypos + h,   1, 0 }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        cursorX += (ch.Advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


KML::Text::Text(std::string key) {
	shader = text_shader;
	font = key;
}

void KML::Text::Draw() {
        assert(shader);
    auto it = g_fonts.find(font);
    if(it == g_fonts.end()) return;

    glm::mat4 model = glm::mat4(1.0f), view = glm::mat4(1.0f), proj  = glm::ortho(0.0f, __KML::LOG_SCREEN_WIDTH, 0.0f, __KML::LOG_SCREEN_HEIGHT, -1.0f, 1.0f);

    // Calculamos ancho y alto total del texto
    float textWidth = 0.0f;
    float textHeight = 0.0f;
    const auto& chars = it->second.glyphs;
    for(char c : text) {
        const Character& ch = chars.at(c);
        textWidth += (ch.Advance >> 6);
        textHeight = std::max(textHeight, (float)ch.Size.y);
    }

    // Ajuste por anchor
    float anchorOffsetX = -textWidth * anchor.x;
    float anchorOffsetY = -textHeight * anchor.y;

    // Posición + rotación + escala + anchor
    model = glm::translate(model, glm::vec3(pos.x, pos.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0,0,1));
    model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));
    model = glm::translate(model, glm::vec3(anchorOffsetX, anchorOffsetY, 0.0f));

    if(camera) {
	    view = glm::translate(glm::mat4(1.0f), glm::vec3(-camera->pos.x, -camera->pos.y, 0.0f));
        if(camera->rotation != 0.0f) view = glm::rotate(view, glm::radians(camera->rotation), glm::vec3(0,0,1));
    }

    Vec4f finalColor = HSVtoRGBA(color);
    finalColor.w = (float)(100 - Clamp<int>(transparency, 0, 100)) / 100.0f;

    glUseProgram(KML::GetShaderID(shader));
    SetUniform_3f("textColor", shader, finalColor.x, finalColor.y, finalColor.z);
    SetUniform_1i("text", shader, 0);
    glUniformMatrix4fv(GetShaderUniformL(shader, "MVP"), 1, GL_FALSE, glm::value_ptr(proj * view * model));

    // Dibujamos en coordenadas locales (0,0) y dejamos que la matriz MVP haga la posición + rotación + escala
    renderText(it, text, 1.0f);
}
