#include "KML/SFX.h"
#include "KML/Utils.h"

#include "__KML/graphics.h"

KML::Particle KML::GenRandParticle() {
    Particle p;
    // Posición inicial dentro de un área de emisión
    p.pos = KML::Vec3f{
        KML::RandFloat(400.0f, 600.0f),  // eje X central
        KML::RandFloat(300.0f, 500.0f),  // eje Y central
        0.0f
    };

    // Velocidad inicial moderada
    p.vel = KML::Vec3f{
        KML::RandFloat(-50.0f, 50.0f),   // permite moverse a la izquierda o derecha
        KML::RandFloat(50.0f, 150.0f),   // dirección hacia arriba (como humo/partículas flotantes)
        0.0f
    };

    // Aceleración pequeña para suavizar el movimiento
    p.accel = KML::Vec3f{
        KML::RandFloat(-5.0f, 5.0f),
        KML::RandFloat(-10.0f, 0.0f),    // ligera gravedad hacia abajo
        0.0f
    };

    p.size = KML::RandFloat(5.0f, 20.0f);   // tamaño moderado
    p.life = KML::RandFloat(2.0f, 4.0f);    // vida inicial más larga
    p.maxLife = p.life;                      // maxLife igual a life para interpolar colores suavemente

    return p;
}

void KML::DrawParticle(Shader* shader, int count, Particle& p) {
    float dt = 0.016f; // frame time aproximado a 60 FPS
    p.vel.x += p.accel.x * dt;
    p.vel.y += p.accel.y * dt;

    // Actualizamos posición con velocidad
    p.pos.x += p.vel.x * dt;
    p.pos.y += p.vel.y * dt;

    // Reducimos vida
    p.life -= dt;

    SetUniform_3fv("particlePos", shader, p.pos);
    SetUniform_3fv("velocity", shader, p.vel);
    SetUniform_3fv("acceleration", shader, p.accel);
    SetUniform_1f("size", shader, p.size);
    SetUniform_1f("life", shader, p.life);
    SetUniform_1f("maxLife", shader, p.maxLife);

    SetUniform_4f("startColor", shader, 1, 0.5, 0, 1);
    SetUniform_4f("endColor", shader, 0.2, 0.2, 0.2, 0);

    KML::SetUniform_1f("radius", shader, KML::RandFloat(0.2f, 0.8f));
    KML::SetUniform_1f("softness", shader, KML::RandFloat(0.2f, 0.8f));
    __KML::draw_rect(shader);
}