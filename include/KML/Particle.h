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
    /**
     * @struct     Particle
     * @brief      Represents a single particle
     */
    struct Particle {
        Vec2f pos;
        Vec3f color;
        Vec2f vel;
        Vec2f acc;
        float size;
        float time;
        float maxTime;
        Particle() {}
    };

    /**
     * @class      ParticleGroup Particle.h Particle
     * @brief      This class describes a particle group.
     * 
     * This class must be inherited since it has pure virtual functions
     * If you do not set the shader or shape pointers, default will be used
     * You may optionally use a texture, only if your particle shader supports this
     */
    class KML_API ParticleGroup {
      public:
        Shader* shader = nullptr;
        Shape* shape = nullptr;
        Camera* camera = nullptr;
        Texture* tex = nullptr;

        ParticleGroup(Shader* shader);

        /**
         * @brief      Adds particles to the group
         *
         * @param[in]  count     Particle n
         * @param      userData  User data pointer that will be passed to the custom generator
         */
        void Generate(int count, void* userData);

        /**
         * @brief      Updates adn draws all the particles in the screen
         * 
         *             DeltaTime is saved as a protected member in this class so you can use it with @ref CustomRenderer()
         *
         * @param[in]  deltaTime  Delta time (s)
         */
        void Draw(double deltaTime);

        /**
         * @brief      Will be called before adding each generated particle to the vector, define this function to set the particle's attribute
         *
         * @param      particle  Current particle
         * @param      userData  User data pointer given to @ref Generate()
         */
        virtual void CustomGenerator(Particle& particle, void* userData) = 0;

        /**
         * @brief      Will be called before drawing vertices to the screen, set the physics, shader, uniforms...
         *
         *             The particle's transformation matrix is calculated in @ref Draw(), so your shader must include "proj", "view", "model" matrix uniforms
         *
         * @param      particle  Current particle
         */
        virtual void CustomRenderer(Particle& particle) = 0;
      protected:
        std::vector<Particle> _particles;
        float _deltaTime;
    };
}

#endif