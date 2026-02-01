#include "KML/KML.h"

#include <iostream>
#include <string>

#include <fmt/core.h>
#include <fmt/format.h>

void myFunc(void* numero) {
    int* n = (int*)numero;
    printf("Function pointer numero: %d\n", *n);
}

class ParticulaFuego : public KML::ParticleGroup {
public:
    ParticulaFuego(KML::Shader* s) : KML::ParticleGroup{s} {}

    void CustomGenerator(KML::Particle& p, void* ptr) {
        float* size_ptr = (float*)(ptr);
        float size = *size_ptr;

        KML::Vec2f o{400.0f, 300.0f}, s{100.0f, 200.0f};
        float minT = 6.0, maxT = 12.0;
        p.pos = o + KML::Vec2f{
            KML::RandFloat(0.0f, s.x),
            KML::RandFloat(0.0f, s.y)
        };

        // Velocidad inicial moderada
        p.vel = KML::Vec2f{
            KML::RandFloat(-5.0f, 5.0f),   // permite moverse a la izquierda o derecha
            KML::RandFloat(-5.0f, 5.0f)   // dirección hacia arriba (como humo/partículas flotantes)
        };

        // Aceleración pequeña para suavizar el movimiento
        p.acc = KML::Vec2f{
            KML::RandFloat(-1.0f, 1.0f),
            KML::RandFloat(-1.0f, 1.0f)    // ligera gravedad hacia abajo
        };

        p.size = KML::RandFloat(1.0f, size);   // tamaño moderado
        p.maxTime = KML::RandFloat((float)minT, (float)maxT);    // vida inicial más larga     
        p.color = KML::Vec3f {KML::RandFloat(0.0f, 1.0f), KML::RandFloat(0.0f, 1.0f), KML::RandFloat(0.0f, 1.0f)};    
        p.time = p.maxTime;  
    }

    void CustomRenderer(KML::Particle& p) {
        p.vel += p.acc * _deltaTime;
        p.pos += p.vel * _deltaTime;
        p.time -= _deltaTime;

        KML::SetUniform_4f("tint", shader, p.color.x, p.color.y, p.color.z, 1.0f);
        KML::SetUniform_1i("uTex", shader, 0);
        KML::SetUniform_1i("useTex", shader, tex > 0 ? 1 : 0);
    }
};

bool keyDown(int k) {
    if(KML::GetKey(k) == KML::KeyState::PRESS || KML::GetKey(k) == KML::KeyState::HOLD) return true;
    return false;
}

int main(void) {
    std::cout << "cwd: " << std::filesystem::current_path() << "\n";
    KML::Clock g_clock;
    
    KML::CreateWindow(800, 600, "KML Window", KML::RESIZABLE | KML::ENABLE_VSYNC | KML::MSAA8 | KML::GL_CONTEXT_LATEST);

    KML::Sprite surface{KML::Vec4f(10.0f, 10.0f, 100.0f, 100.0f)};
    surface.color = KML::HSV_v3f(293, 83, 93);
    surface.anchor = KML::Vec2f(0.5f, 0.5f);

    std::string font0 = KML::LoadFont("assets/arial.ttf", 48);

    KML::Shader* shaderParticles = KML::CreateShader("assets/particulas/vert.glsl", "assets/particulas/frag.glsl");

    KML::Text text0{font0};
    text0.text = "Hola, KML!";
    //text0.anchor = KML::Vec2f(0.5, 0.5);

    int numerito = 998899;
    int* numPtr = &numerito;
    KML::Timer timer0{g_clock, 10.0, myFunc, (void*)numPtr};
    timer0.Start();

    KML::Texture tex0 = KML::LoadTexture("assets/images.png");

    ParticulaFuego particle0{nullptr};
    particle0.tex = tex0;

    KML::Camera cam0{KML::Vec2f(0, 0), KML::Vec2f(1, 1), 0.0};
    surface.camera = &cam0;
    text0.camera = &cam0;

    g_clock.Tick();
    while(KML::ProcessEvents()) {
        double deltaTime = g_clock.Tick();

        if(KML::GetKey(KML_KEY_ESCAPE) == KML::KeyState::PRESS) {
            if (!KML::GetMouseCaptureState())
                KML::Event(KML::WindowEvent::EXIT, 1);
            else 
                KML::Event(KML::WindowEvent::MOUSE_CAPTURED, 0);
        }

        if(keyDown(KML_KEY_W)) text0.pos += KML::Vec2f{0.0f, 1.0f};
        if(keyDown(KML_KEY_S)) text0.pos += KML::Vec2f{0.0f, -1.0f};
        if(keyDown(KML_KEY_A)) text0.pos += KML::Vec2f{-1.0f, 0.0f};
        if(keyDown(KML_KEY_D)) text0.pos += KML::Vec2f{1.0f, 0.0f};
        if(keyDown(KML_KEY_LEFT)) text0.rotation += 10.0f;
        if(keyDown(KML_KEY_RIGHT)) text0.rotation -= 10.0f;
        if(keyDown(KML_KEY_UP)) text0.scale += KML::Vec2f{1.0f, 1.0f};
        if(keyDown(KML_KEY_DOWN)) text0.scale -= KML::Vec2f{1.0f, 1.0f};
        if(keyDown(KML_KEY_F)) {
            text0.color.y += 10.0f;
            text0.color.x += 5.0f;
        }
        if(keyDown(KML_KEY_V)) {
            text0.color.y -= 10.0f;
            text0.color.x -= 5.0f;
        }

        if(KML::GetKey(KML_KEY_X) == KML::KeyState::PRESS) {
            text0.color = KML::HSV_v3f(0, 100, 100);
            text0.anchor = {0.0f, 0.0f};
            text0.pos = {0.0f, 0.0f};
            text0.scale = {1.0f, 1.0f};
            text0.rotation = 0.0f;
            KML::UnloadFont("assets/arial.ttf:48");
        }

        if(KML::GetKey(KML_KEY_SPACE) == KML::KeyState::PRESS) {
            float size = 30.0f;

            particle0.Generate(30, (void*)(&size));
        }

        if(KML::GetKey(KML_KEY_R) == KML::KeyState::PRESS) {
            KML::ReloadShader(shaderParticles);
        }

        if(KML::GetKey(KML_KEY_P) == KML::KeyState::PRESS) fmt::print("pos: {}, {} scale: {}, {} rot: {})\n", cam0.pos.x, cam0.pos.y, cam0.size.x, cam0.size.y, cam0.rotation); 

        if(KML::GetMouseButton(KML_MOUSE_BUTTON_LEFT) == KML::KeyState::PRESS) 
            KML::Event(KML::WindowEvent::MOUSE_CAPTURED, 1);

        if(keyDown(KML_KEY_U)) cam0.pos += KML::Vec2f{0.0f, 1.0f};
        if(keyDown(KML_KEY_J)) cam0.pos += KML::Vec2f{0.0f, -1.0f};
        if(keyDown(KML_KEY_H)) cam0.pos += KML::Vec2f{-1.0f, 0.0f};
        if(keyDown(KML_KEY_K)) cam0.pos += KML::Vec2f{1.0f, 0.0f};
        if(keyDown(KML_KEY_G)) cam0.rotation += 5.0f;
        if(keyDown(KML_KEY_B)) cam0.rotation -= 5.0f;

        surface.pos = text0.pos;
        surface.rotation = text0.rotation;
        surface.anchor = text0.anchor;

        timer0.Query();

        surface.Draw();
        text0.Draw();
        particle0.Draw(deltaTime);
        KML::PresentFrame();
    }
    KML::Quit();
    return 0;
}