#include "KML/KML.h"

#include <iostream>
#include <string>

#include <fmt/core.h>
#include <fmt/format.h>


bool keyDown(int k) {
    if(KML::GetKey(k) == KML::KeyState::PRESS || KML::GetKey(k) == KML::KeyState::HOLD) return true;
    return false;
}

int main(void) {
    std::cout << "cwd: " << std::filesystem::current_path() << "\n";
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
    KML::Texture tex0 = KML::LoadTexture("assets/images.png");
    KML::LoadTexture("assets/model.png");
    KML::Shader* shader = KML::CreateShader(std::filesystem::path("assets/particulas/vert.glsl"), std::filesystem::path("assets/particulas/frag.glsl")); 

    KML::LoadSound("assets/test_clean.ogg", false, false);
    KML::LoadSound("assets/voice.wav", false, false);
    KML::LoadSound("assets/test.wav", true, false);

    KML::LoadFont("assets/arial.ttf");

    KML::Surface background(KML::Vec4f{0.0f, 0.0f, 800.0f, 600.0f});
    background.SetColor_RGB(118, 144, 166);

    KML::Surface surface("assets/images.png", {0.0f, 0.0f, 1.0f, 1.0f}, {0.5f, 0.5f}, 0.0f);
    surface.transparency = 20;

    KML::Surface yerba("assets/model.png", {100.0f, 100.0f, 100.0f, 100.0f});
    
    KML::Surface srf(KML::Vec4f{30.0f, 50.0f, 70.0f, 10.0f});
    srf.color = KML::Vec3f(110.0f, 30.0f, 40.0f);
    srf.shape = &shape0;

    KML::Surface srf2(KML::Vec4f{186.0f, 400.0f, 200.0f, 140.0f});
    srf2.SetColor_RGB(255, 107, 30);
    srf2.transparency = 30;

    KML::Layer layer0;
    layer0.Add(srf);
    layer0.Add(srf2);

    KML::Layer layer1;
    layer1.Add(surface);
    layer1.Add(yerba);

    fmt::print("Time before main loop: {} s\n", stopwatch.Query());

    double accum = 0;
    int fps = 0;
    float sound_p = 1.0f;
    float sound_v = 1.0f;

    KML::SoundPool soundP;
    KML::PlayMusic("assets/voice.wav");

    surface.tex = KML::GetBitmap("assets/arial.ttf", "Hola, KML!");

    clock.Tick();
    while(KML::ProcessEvents()) {
        //if(temp_1 <= 3) fmt::print("This frame ({}) took {} s\n", temp_1, clock.Tick());

        double deltaTime = clock.Tick();
        accum += deltaTime;
        fps++;

        if(KML::GetKey(KML_KEY_ESCAPE) == KML::KeyState::PRESS) {
            if (!KML::GetMouseCaptureState())
                KML::Event(KML::WindowEvent::EXIT, 1);
            else 
                KML::Event(KML::WindowEvent::MOUSE_CAPTURED, 0);
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

        if(KML::GetKey(KML_KEY_P) == KML::KeyState::PRESS) fmt::print("pos: {}, {} scale: {}, {} rot: {}, anchor: {}, {})\n", surface.pos.x, surface.pos.y, surface.scale.x, surface.scale.y, surface.rotation, surface.anchor.x, surface.anchor.y); 

        if(KML::GetKey(KML_KEY_R) == KML::KeyState::PRESS) { 
            printf("reloading shader!");
            KML::ReloadShader(shader);
        }

        if(KML::GetKey(KML_KEY_SPACE) == KML::KeyState::PRESS) {
            soundP.Stop();
        }

        if(keyDown(KML_KEY_6)) {
            sound_v -= 0.05;
            KML::SetGlobalVolume(sound_v);
        }

        if(keyDown(KML_KEY_7)) {
            sound_v += 0.05;
            KML::SetGlobalVolume(sound_v);
        }

        if(keyDown(KML_KEY_8)) {
            sound_p -= 0.1;
            KML::SetGlobalPitch(sound_p);
        }

        if(keyDown(KML_KEY_9)) {
            sound_p += 0.1;
            KML::SetGlobalPitch(sound_p);
        }

        if(KML::GetKey(KML_KEY_0) == KML::KeyState::PRESS) {
            soundP.Add(KML::Sound{"assets/test_clean.ogg", KML::RandFloat(0.0f, 1.0f), KML::RandFloat(0.0f, 2.0f)});
        }

        if(KML::GetKey(KML_KEY_1) == KML::KeyState::PRESS) {
            //soundP.Add(KML::Sound{"assets/test.wav"});
            KML::ResetMusic("assets/voice.wav");
            KML::PlayMusic("assets/voice.wav");
        }

        if(KML::GetMouseButton(KML_MOUSE_BUTTON_LEFT) == KML::KeyState::PRESS) 
            KML::Event(KML::WindowEvent::MOUSE_CAPTURED, 1);

        if(KML::GetKey(KML_KEY_M) == KML::KeyState::PRESS) {
            KML::Event(KML::WindowEvent::HIDE, 1);
        } 

        background.Draw();
        KML::DrawLayers();
        soundP.Play();
        soundP.Update();

        KML::PresentFrame();

        if(accum >= 1.0) {
            KML::SetWindowTitle(fmt::format("KML Window | {} FPS", fps).c_str());
            accum -= 1.0;
            fps = 0;
        }
    }

    KML::DeleteShader(shader);
    KML::UnloadTexture("assets/images.png");
    fmt::print("This program took {} s (using stopwatch: {})\n", clock.Now() - time_start, stopwatch.Stop());
    KML::Terminate();
    return 0;
}