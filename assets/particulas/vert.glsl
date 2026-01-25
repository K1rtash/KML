#version 330 core

layout(location = 0) in vec3 aPos;   // posición del quad
layout(location = 1) in vec2 aUV;    // coordenadas UV del quad

out vec2 uvCoords;
out float lifeRatio;

uniform vec3 particlePos;     // posición central de la partícula
uniform vec3 velocity;        // velocidad inicial
uniform vec3 acceleration;    // aceleración
uniform float size;           // tamaño de la partícula
uniform float life;           // vida restante
uniform float maxLife;        // vida máxima

uniform mat4 proj;            // proyección

void main()
{
    // Tiempo relativo de la partícula
    lifeRatio = life / maxLife;

    // Calcula desplazamiento
    vec3 displacement = velocity * lifeRatio + 0.5 * acceleration * lifeRatio * lifeRatio;

    // Escala el quad
    vec3 pos = particlePos + displacement + aPos * size;

    gl_Position = vec4(pos, 1.0);
    uvCoords = aUV;
}
