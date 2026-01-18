#version 330 core
out vec4 FragColor;

in vec2 uvCoords;

uniform sampler2D uTex;
uniform vec4 tint;

void main()
{
    vec4 texColor = texture(uTex, uvCoords);
    texColor *= tint;
    texColor *= vec4(0.4, 0.4, 0.4, 1);
    if(texColor.a < 0.1) discard;
    FragColor = texColor;
}