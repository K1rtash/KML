#include "KML/KML.h"

#include <string>
#include <iostream>

bool keyDown(int k) {
    if(KML::GetKey(k) == KML::KeyState::PRESS || KML::GetKey(k) == KML::KeyState::HOLD) return true;
    return false;
}

int main(void) {
    KML::Clock clock;
    KML::Stopwatch stopwatch(clock);
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

    KML::Surface background(KML::Vec4f{0.0f, 0.0f, 800.0f, 600.0f});
    background.SetColor_RGB(118, 144, 166);

    KML::Surface surface{KML::Vec4f(400.0f, 300.0f, 300.0f, 300.0f)};
    surface.transparency = 25;
    surface.anchor = KML::Vec2f(0.5f, 0.5f);
    surface.shape = &shape0;

    surface.SetColor_RGB(255, 107, 30);

   
    double accum = 0;
    int fps = 0;

    clock.Tick();
    while(KML::ProcessEvents()) {
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

        if(KML::GetMouseButton(KML_MOUSE_BUTTON_LEFT) == KML::KeyState::PRESS) 
            KML::Event(KML::WindowEvent::MOUSE_CAPTURED, 1);

        background.Draw();
        surface.Draw();

        KML::PresentFrame();

        if(accum >= 1.0) {
            std::string title = "KML Window | " + std::to_string(fps) + " FPS";
            KML::SetWindowTitle(title.c_str());
            accum -= 1.0;
            fps = 0;
        }
    }

    std::cout << "This program took " << stopwatch.Stop() << " seconds" << std::endl;
    KML::Terminate();
    return 0;
}