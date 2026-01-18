#ifndef KML_SFX_H
#define KML_SFX_H

#include "Vector.h"
#include "Graphics.h"

namespace KML {
    struct Particle {
        KML::Vec3f pos;
        KML::Vec3f vel;
        KML::Vec3f accel;
        float size;
        float life;
        float maxLife;
    };

    void DrawParticle(Shader* shader, int count, Particle& p);
    Particle GenRandParticle();
}

#endif