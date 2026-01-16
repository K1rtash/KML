#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 uvCoords;

uniform mat4 model;

void main() {
    uvCoords = aUV;
    vec4 pos = vec4(aPos, 1.0);
    gl_Position = model * pos;
}