#ifndef KML_SFX_H
#define KML_SFX_H

#include "ABI.h"

#include "Vector.h"
#include "Graphics.h"
#include "Time.h"
#include "Surface.h"

namespace KML {
    class KML_API Particle : public Surface {
        Vec2f vel;
        Vec2f acc;
        float size;
        double maxLife;
        int count;
        Shader* shader;
        Shape* shape;
        Texture tex;
        Particle(Clock& clock, double life);
        void Draw(double deltaTime);
      private:
        Timer timer;
        bool started = false;
    };

    //KML_API Particle GenRandParticle(double deltaTime);
}

#endif