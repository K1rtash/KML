#include "KML/KML.h"

#include <iostream>
#include <string>

#include <fmt/core.h>
#include <fmt/format.h>
#include <ft2build.h>
#include FT_FREETYPE_H

void myFunc(void* numero) {
    int* n = (int*)numero;
    printf("Function pointer numero: %d\n", *n);
}


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

    KML::ParticleGroup particle0{nullptr};

    int dibujaParticulas = 0;
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
            particle0.Generate(10, 2.0, 6.0, KML::Vec2f{400.0f, 300.0f}, KML::Vec2f{5.0f, 2.0f});
            dibujaParticulas = 1;
        }

        if(KML::GetKey(KML_KEY_R) == KML::KeyState::PRESS) {
            KML::ReloadShader(shaderParticles);
        }

        if(KML::GetKey(KML_KEY_P) == KML::KeyState::PRESS) fmt::print("pos: {}, {} scale: {}, {} rot: {}, anchor: {}, {})\n", text0.pos.x, text0.pos.y, text0.scale.x, text0.scale.y, text0.rotation, text0.anchor.x, text0.anchor.y); 

        if(KML::GetMouseButton(KML_MOUSE_BUTTON_LEFT) == KML::KeyState::PRESS) 
            KML::Event(KML::WindowEvent::MOUSE_CAPTURED, 1);

        surface.pos = text0.pos;
        surface.rotation = text0.rotation;
        surface.anchor = text0.anchor;

        timer0.Query();

        surface.Draw();
        text0.Draw();
        if(dibujaParticulas) particle0.Draw(deltaTime);
        KML::PresentFrame();
    }
    KML::Quit();
    return 0;
}