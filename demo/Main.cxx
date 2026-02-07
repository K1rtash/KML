#include <iostream>
#include <KML/KML.h>

int main() {
    KML::CreateWindowP(410, 280, "Conditioned Subject", 410, 280, 3, 3, KML::MSAA8 | KML::RESIZABLE);

    KML::Clock clock0{};

    KML::Shader* shader0 = KML::CreateShader("assets/shaders/logo.vs", "assets/shaders/logo.fs");

    KML::Texture tex0 = KML::LoadTexture("assets/logo.png");

    float quad[] = {
     //  x     y     z      u     v
       -1.f, -1.f, 0.f,   0.f, 0.f,
        1.f, -1.f, 0.f,   1.f, 0.f,
        1.f,  1.f, 0.f,   1.f, 1.f,
       -1.f,  1.f, 0.f,   0.f, 1.f
    };

    unsigned int quadIdx[] = {
        0, 1, 2,
        2, 3, 0
    };


    KML::Shape shape0{quad, sizeof(quad), quadIdx, sizeof(quadIdx)};

    KML::Sprite sprite0;
    sprite0.pos = KML::Vec2f(0.0f, 0.0f);
    //sprite0.shape = &shape0;
    //sprite0.shader = shader0;
    sprite0.scale = KML::Vec2f(100.0f, 100.0f);
    sprite0.color = KML::HSV_v3f(219, 83, 24);

    while(KML::ProcessEvents()) {
        float time = (float)clock0.Now();

        KML::SetUniform_1f("u_time", shader0, time);
        KML::SetUniform_2fv("u_resolution", shader0, KML::Vec2f(410.0f, 280.0f));
        KML::SetUniform_3fv("u_logoColor", shader0, KML::Vec3f(1.0f, 0.0f, 0.0f));

        KML::UseShader(shader0);
        KML::BindTexture(tex0, 0);
        shape0.Draw();
        sprite0.Draw();

        if(KML::GetKey(KML_KEY_ESCAPE) == KML::KeyState::PRESS) {
            KML::Event(KML::WindowEvent::EXIT, 1);
        }

        if(KML::GetKey(KML_KEY_R) == KML::KeyState::PRESS) {
            KML::ReloadShader(shader0);
        }

        KML::PresentFrame(1.0, 1.0, 1.0);
    }

    KML::Quit();
    return 0;
}