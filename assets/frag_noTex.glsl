#version 330 core
out vec4 FragColor;

in vec2 uvCoords;

uniform vec4 tint;

void main()
{
    FragColor = tint * vec4(1.0);
}