#version 330 core

in vec2 uvCoords;
in float lifeRatio;

out vec4 FragColor;

uniform vec4 startColor;   // color al nacer
uniform vec4 endColor;     // color al morir
uniform float radius;      // radio de la partícula (0..1)
uniform float softness;    // suavidad del borde

void main()
{
    // Gradiente circular para alpha suave
    vec2 pos = uvCoords * 2.0 - 1.0;
    float dist = length(pos);
    float alpha = smoothstep(radius, radius - softness, dist);

    // Interpolación de color según vida
    vec4 color = mix(endColor, startColor, lifeRatio);

    FragColor = vec4(color.rgb, color.a * alpha);

    if(alpha <= 0.0)
        discard;
}
