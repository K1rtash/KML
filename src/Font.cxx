#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <iostream>
#include <string>


#include "KML/Font.h"
#include "__KML/graphics.h"


struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

std::map<char, Character> Characters;
GLuint VAO, VBO;
FT_Library ft;

void loadFontAsFace(const char* file, int pixel_size = 48) {
	FT_Face face;
    if (FT_New_Face(ft, file, 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, pixel_size);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
	  
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
	    Characters.insert(std::pair<char, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
    //FT_Done_FreeType(ft);
}

void KML::LoadFont(const char* file) {
	loadFontAsFace(file, 48);
}


void KML::RenderText(Shader* shader, std::string text, float x, float y, float scale, Vec3f color) {
    // activate corresponding render state	
    GLuint shader_id = GetShaderID(shader);
    glUseProgram(shader_id);
    glUniform3f(glGetUniformLocation(shader_id, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
glUniformMatrix4fv(glGetUniformLocation(shader_id, "projection"),
                   1, GL_FALSE, glm::value_ptr(proj));

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
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
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
	    "gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
	    "TexCoords = vertex.zw;\n"
		"}\n";

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