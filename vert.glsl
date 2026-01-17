#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 uvCoords;
uniform mat4 model;
uniform mat4 proj;

void main() {
    uvCoords = aUV;
    gl_Position = proj * model * vec4(aPos, 1.0);
};