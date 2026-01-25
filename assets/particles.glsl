#version 330 core

// Viene del vertex shader
in vec2 uvCoords;

// Uniforms
uniform vec4 color;       // Color de la partícula (RGBA)
uniform float radius;     // Radio de la partícula (0..1)
uniform float softness;   // Suavidad del borde (0..1)

// Salida
out vec4 FragColor;

void main()
{
    // Centra las coordenadas de 0..1 a -1..1
    vec2 pos = uvCoords * 2.0 - 1.0;
    
    // Distancia al centro
    float dist = length(pos);

    // Gradiente de transparencia: suave en el borde
    float alpha = smoothstep(radius, radius - softness, dist);

    FragColor = vec4(color.rgb, color.a * alpha);

    // Opcional: descarta totalmente los píxeles fuera del círculo
    if(alpha <= 0.0)
        discard;
}