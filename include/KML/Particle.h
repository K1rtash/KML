#ifndef KML_SFX_H
#define KML_SFX_H

#include "ABI.h"

#include "Vector.h"
#include "Graphics.h"

namespace KML {
    struct KML_API Particle {
        KML::Vec3f pos;
        KML::Vec3f vel;
        KML::Vec3f accel;
        float size;
        float life;
        float maxLife;
    };

    KML_API void DrawParticle(Shader* shader, int count, Particle& p);
    KML_API Particle GenRandParticle();
}

#endif