#include "__KML/graphics.h"

#include "KML/Shape.h"

namespace __KML {
    KML::Shape* defaultShape;
    KML::Shader* defaultShader;
}

void __KML::GenerateDefaultMembers() {
    const char* vertex_src = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aUV;\n"
        "out vec2 uvCoords;\n"
        "uniform mat4 model;\n"
        "uniform mat4 proj;\n"
        "uniform mat4 view;\n"
        "void main() {\n"
        "uvCoords = aUV;\n"
        "gl_Position = proj * view * model * vec4(aPos, 1.0);\n"
        "}";

    const char* fragment_src = 
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec2 uvCoords;\n"
        "uniform sampler2D uTex;\n"
        "uniform vec4 tint;\n"
        "uniform bool useTex;"
        "void main()\n"
        "{\n"
        "vec4 finalColor;\n"
        "if(useTex) {\n"
        "vec4 texColor = texture(uTex, uvCoords);\n"
        "texColor *= tint;\n"
        "if(texColor.a < 0.1) discard;\n"
        "finalColor = vec4(texColor.rgb, texColor.a * tint.w) * tint;;\n"
        "} else {\n"
        "finalColor = tint;\n"
        "}\n"
        "FragColor = finalColor;\n"
        "}";

    float vertices[] = {
        0.5f,  0.5f, 0.0f,     1.0f, 1.0f,    // top right
        0.5f, -0.5f, 0.0f,     1.0f, 0.0f,    // bottom right
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,    // bottom left
        -0.5f,  0.5f, 0.0f,     0.0f, 1.0f     // top left 
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    defaultShape = KML::CreateShape(vertices, sizeof(vertices), indices, sizeof(indices));
    KML::VertexFloatAttribute(defaultShape, 0, 3, 5, 0);
    KML::VertexFloatAttribute(defaultShape, 1, 2, 5, 3);    

    defaultShader = KML::CreateShaderFS(vertex_src, fragment_src);
}