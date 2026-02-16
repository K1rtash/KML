#include <iostream>
#include <KML/KML.h>
#include <glad/glad.h>

int width = 320, height = 180;

int main() {
    KML::InitWindow(width, height, "Conditioned Subject", KML::RESIZABLE | KML::ENABLE_VSYNC | KML::MSAA8 | KML::GL_CONTEXT_LATEST);

    KML::Clock clock0{};

    KML::Shader* shader0 = KML::CreateShader("assets/shaders/logo.vs", "assets/shaders/logo.fs");
    KML::Shader* framebufferProgram = KML::CreateShader("assets/shaders/fb.vs", "assets/shaders/fb.fs");
    KML::SetUniform_1i("screenTexture", framebufferProgram, 0);

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

    KML::Framebuffer* framebuff = KML::CreateFramebuffer(width, height);

    while(KML::ProcessEvents()) {
        float time = (float)clock0.Now();

        KML::SetUniform_1f("u_time", shader0, time);
        KML::SetUniform_2fv("u_resolution", shader0, KML::Vec2f((float)width, (float)height));
        KML::SetUniform_3fv("u_logoColor", shader0, KML::Vec3f(1.0f, 0.0f, 0.0f));

        KML::BindFramebuffer(framebuff, KML::Vec3f(0.07f, 0.13f, 0.17f));

        KML::UseShader(shader0);
        KML::BindTexture(tex0, 0);
        shape0.Draw();

        if(KML::GetKey(KML_KEY_ESCAPE) == KML::KeyState::PRESS) {
            KML::Event(KML::WindowEvent::EXIT, 1);
        }

        if(KML::GetKey(KML_KEY_R) == KML::KeyState::PRESS) {
            KML::ReloadShader(shader0);
            KML::ReloadShader(framebufferProgram);
        }

        //
        int w, h;
        KML::GetWindowSize(&w, &h);
        KML::SetWindowViewport(w, h);

        KML::DrawFramebuffer(framebuff, framebufferProgram, w, h);
        KML::SwapBuffers();
        //KML::PresentFrame(KML::Vec3f(0.5f));
    }
    KML::DestroyFramebuffer(framebuff);
    KML::Quit();
    return 0;
}