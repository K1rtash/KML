#ifndef KML_SFX_H
#define KML_SFX_H

#include <vector>

#include "ABI.h"

#include "Vector.h"
#include "Graphics.h"
#include "Time.h"
#include "Surface.h"

/**
 * @file    Particle.h
 * @brief   Abstracts SFX
 */

namespace KML {
    struct Particle {
        Vec2f pos;
        Vec3f color;
        Vec2f vel;
        Vec2f acc;
        float size;
        float time;
        float maxTime;
    };

    class KML_API ParticleGroup {
      public:
        Shader* shader = nullptr;
        Shape* shape = nullptr;
        Texture tex = 0;
        Vec2f origin;
        ParticleGroup(Shader* shader, int count);
        void Generate(double minTime, double maxTime, Vec2f origin, Vec2f spread);
        void Draw(double deltaTime);
      private:
        //Particle* particles;
        int count = 10;
        std::vector<Particle> particles;

    };

    KML_API Particle GenRandParticle(double minTime, double maxTime, Vec2f origin, Vec2f spread);
}

#endif