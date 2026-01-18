#include "KML/kml.h"
#include "KML/Layers.h"
#include "KML/SFX.h"
#include "KML/Time.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Particula : public KML::Surface {
    public:
    KML::Vec3f vel;
    KML::Vec3f accel;
    float size;
    float life;
    float maxLife;
    void Draw() {
        float dt = 0.016f; // frame time aproximado a 60 FPS
        vel.x += accel.x * dt;
        vel.y += accel.y * dt;

        // Actualizamos posición con velocidad
        pos.x += vel.x * dt;
        pos.y += vel.y * dt;

        // Reducimos vida
        life -= dt;

        SetUniform_3fv("particlePos", shader, KML::Vec3f{pos.x, pos.y, 0.0f});
        SetUniform_3fv("velocity", shader, vel);
        SetUniform_3fv("acceleration", shader, accel);
        SetUniform_1f("size", shader, size);
        SetUniform_1f("life", shader, life);
        SetUniform_1f("maxLife", shader, maxLife);

        SetUniform_4f("startColor", shader, 1, 0.5, 0, 1);
        SetUniform_4f("endColor", shader, 0.2, 0.2, 0.2, 0);

        KML::SetUniform_1f("radius", shader, KML::RandFloat(0.2f, 0.8f));
        KML::SetUniform_1f("softness", shader, KML::RandFloat(0.2f, 0.8f));

        shape->Use();
    }
};

Particula nuevaP(KML::Shader* s, KML::Shape* shape) {
    Particula p;
    // Posición inicial dentro de un área de emisión
    p.pos = KML::Vec2f{
        KML::RandFloat(400.0f, 600.0f),  // eje X central
        KML::RandFloat(300.0f, 500.0f),  // eje Y central
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

    p.shader = s;
    p.shape = shape;
    return p;
}

bool keyDown(int k) {
    if(KML::GetKey(k) == KML::KeyState::PRESS || KML::GetKey(k) == KML::KeyState::HOLD) return true;
    return false;
}

int gtrnd(int min, int max) {
    return rand () % (max - min + 1) + min;
}

int main(void) {
    srand(time(0));

    KML::Clock clock;
    KML::Stopwatch stopwatch(clock);
    double time_start = clock.Now();
    stopwatch.Start();

    KML::CreateWindow(800, 600, "KML Window", KML::RESIZABLE | KML::ENABLE_VSYNC | KML::MSAA8 | KML::GL_CONTEXT_LATEST);
    KML::PrintContext();

    float vertices[] = {
        //  x     y    z     u    v
        0.0f,  0.5f, 0.0f, 0.5f, 1.0f,   // cima
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // izquierda
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f    // derecha
    };

    // Índices para un solo triángulo
    unsigned int indices[] = {
        0, 1, 2
    };
    KML::Shape shape0(vertices, sizeof(vertices), indices, sizeof(indices));

    KML::Texture tex0 = KML::LoadTexture("images.png");
    KML::Shader* shader = KML::CreateShader(std::filesystem::path("particulas/vert.glsl"), std::filesystem::path("particulas/frag.glsl")); 

    KML::Surface background(KML::Vec4f{0.0f, 0.0f, 800.0f, 600.0f});
    background.SetColor_RGB(118, 144, 166);

    KML::Surface surface("images.png", {0.0f, 0.0f, 1.0f, 1.0f}, {0.5f, 0.5f}, 0.0f);
    surface.transparency = 20;
    //surface.shape = &shape0;
    KML::LoadTexture("model.png");
    KML::Surface yerba("model.png", {100.0f, 100.0f, 100.0f, 100.0f});
    
    KML::Surface srf(KML::Vec4f{30.0f, 50.0f, 70.0f, 10.0f});
    srf.color = KML::Vec3f(110.0f, 30.0f, 40.0f);

    KML::Surface srf2(KML::Vec4f{186.0f, 400.0f, 200.0f, 140.0f});
    srf2.SetColor_RGB(255, 107, 30);
    srf2.transparency = 30;

    KML::Layer layer0;
    layer0.Add(srf);
    layer0.Add(srf2);
    KML::Layer layer1;
    layer1.Add(surface);
    layer1.Add(yerba);

    std::vector<Particula> particulas;

    std::cout << std::format("Time before main loop: {} s\n", stopwatch.Query());
    int temp_1 = 0;
    clock.Tick();
    while(KML::ProcessEvents()) {
        if(temp_1 <= 3) std::cout << std::format("This frame ({}) took {} s\n", temp_1, clock.Tick());
        temp_1++;

        if(KML::GetKey(KML_KEY_ESCAPE) == KML::KeyState::PRESS) {
            if (!KML::GetMouseCaptureState())
                KML::Close();
            else 
                KML::SetMouseCaptured(false);
        }

        if(keyDown(KML_KEY_W)) surface.pos += KML::Vec2f{0.0f, 1.0f};
        if(keyDown(KML_KEY_S)) surface.pos += KML::Vec2f{0.0f, -1.0f};
        if(keyDown(KML_KEY_A)) surface.pos += KML::Vec2f{-1.0f, 0.0f};
        if(keyDown(KML_KEY_D)) surface.pos += KML::Vec2f{1.0f, 0.0f};
        if(keyDown(KML_KEY_LEFT)) surface.rotation += 10.0f;
        if(keyDown(KML_KEY_RIGHT)) surface.rotation -= 10.0f;
        if(keyDown(KML_KEY_UP)) surface.scale += KML::Vec2f{1.0f, 1.0f};
        if(keyDown(KML_KEY_DOWN)) surface.scale -= KML::Vec2f{1.0f, 1.0f};
        if(keyDown(KML_KEY_F)) {
            surface.color.y += 10.0f;
            surface.color.x += 5.0f;
        }
        if(keyDown(KML_KEY_V)) {
            surface.color.y -= 10.0f;
            surface.color.x -= 5.0f;
        }

        if(KML::GetKey(KML_KEY_X) == KML::KeyState::PRESS) {
            surface.SetColor_RGB(255, 255, 255);
            surface.anchor = {0.0f, 0.0f};
            surface.pos = {0.0f, 0.0f};
            surface.scale = {1.0f, 1.0f};
            surface.rotation = 0.0f;
        }

        if(KML::GetKey(KML_KEY_P) == KML::KeyState::PRESS) std::cout << std::format("pos: {}, {} scale: {}, {} rot: {}, anchor: {}, {})\n", surface.pos.x, surface.pos.y, surface.scale.x, surface.scale.y, surface.rotation, surface.anchor.x, surface.anchor.y); 

        if(KML::GetKey(KML_KEY_R) == KML::KeyState::PRESS) { 
            printf("reloading shader!");
            KML::ReloadShader(shader);
        }

        if(KML::GetKey(KML_KEY_SPACE) == KML::KeyState::PRESS) {
            particulas.clear();
            particulas.push_back(nuevaP(shader, &shape0));
        }

        if(KML::GetMouseButton(KML_MOUSE_BUTTON_LEFT) == KML::KeyState::PRESS) 
            KML::SetMouseCaptured(true);

        background.Draw();
        KML::DrawLayers();

        for(int i = 0; i < particulas.size(); i++) {
            particulas[i].Draw();
        }

        KML::PresentFrame();
    }

    KML::DeleteShader(shader);
    KML::UnloadTexture("images.png");
    KML::Terminate();
    std::cout << std::format("This program took {} s (using stopwatch: {})\n", clock.Now() - time_start, stopwatch.Stop());
    return 0;
}