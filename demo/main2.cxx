#include <iostream>
#include <KML/KML.h>
#include <KML/ResourceManager.h>
#include <glad/glad.h>

int width = 1920, height = 1080;

void removeShaderCallback(KML::Shader* shader, void* uptr) {
    KML::DeleteShader(shader);
}

void removeTexCb(KML::Texture* tex, void* uptr) {

}

int main() {
    KML::InitWindow(width, height, "Conditioned Subject", KML::RESIZABLE | KML::ENABLE_VSYNC | KML::MSAA8 | KML::GL_CONTEXT_LATEST | KML::LOCK_ASPECT);

    KML::Clock clock0{};

    KML::Shader* shader0 = KML::CreateShader("assets/shaders/logo.vs", "assets/shaders/logo.fs");
    KML::Shader* framebufferProgram = KML::CreateShader("assets/shaders/fb.vs", "assets/shaders/fb.fs");
    KML::SetUniform_1i("screenTexture", framebufferProgram, 0);


    KML::ResourceManager <KML::Shader*>shaderManager{nullptr, removeShaderCallback, nullptr};
    KML::ResourceManager <KML::Texture*>texManager{nullptr, removeTexCb, nullptr};

    KML::Texture* tex0 = KML::CreateTexture("assets/logo.png");
    texManager.Add("assets/logo.png", tex0);

    shaderManager.Add("shader0", shader0);

    KML::Sprite sprite0;
    sprite0.scale = KML::Vec2f(32.0f, 18.0f);
    sprite0.SetColor_RGB(255, 215, 200);

    float quad[] = {
     //  x     y     z      u     v
       -1.f, -1.f,   0.f, 0.f,
        1.f, -1.f,   1.f, 0.f,
        1.f,  1.f,   1.f, 1.f,
       -1.f,  1.f,   0.f, 1.f
    };

    unsigned int quadIdx[] = {
        0, 1, 2,
        2, 3, 0
    };

    KML::Shape* shape0 = KML::CreateShape(quad, sizeof(quad), quadIdx, sizeof(quadIdx));
    KML::VertexFloatAttribute(shape0, 0, 3, 4, 0);
    KML::VertexFloatAttribute(shape0, 1, 2, 4, 2);

    KML::Framebuffer* framebuff = KML::CreateFramebuffer(width, height);

    while(KML::ProcessEvents()) {
        float time = (float)clock0.Now();

        if(time >= 3.0f) KML::Event(KML::WindowEvent::HIGHLIGHT, 1);

        KML::SetUniform_1f("u_time", shaderManager.Get("shader0"), time);
        KML::SetUniform_2fv("u_resolution", shaderManager.Get("shader0"), KML::Vec2f(420.0f, 280.0f));
        KML::SetUniform_3fv("u_logoColor", shaderManager.Get("shader0"), KML::Vec3f(1.0f, 0.0f, 0.0f));
        SetUniform_1f("u_fadeLenght", shaderManager.Get("shader0"), 5.0f);
        SetUniform_1f("u_fadeTimer", shaderManager.Get("shader0"), 0);

        KML::BindFramebuffer(framebuff, KML::Vec3f(0.07f, 0.13f, 0.17f));

        KML::UseShader(shaderManager.Get("shader0"));
        KML::BindTexture(texManager.Get("assets/logo.png"), 0);
        KML::DrawShape(shape0);

        if(KML::GetKey(KML_KEY_ESCAPE) == KML::KeyState::PRESS) {
            KML::Event(KML::WindowEvent::EXIT, 1);
        }

        if(KML::GetKey(KML_KEY_R) == KML::KeyState::PRESS) {
            KML::ReloadShader(shaderManager.Get("shader0"));
            KML::ReloadShader(framebufferProgram);
        }

        if(KML::GetKey(KML_KEY_A) == KML::KeyState::PRESS || KML::GetKey(KML_KEY_A) == KML::KeyState::HOLD) sprite0.pos.x -= 1.0f;
        if(KML::GetKey(KML_KEY_D) == KML::KeyState::PRESS || KML::GetKey(KML_KEY_D) == KML::KeyState::HOLD) sprite0.pos.x += 1.0f;
        if(KML::GetKey(KML_KEY_W) == KML::KeyState::PRESS || KML::GetKey(KML_KEY_W) == KML::KeyState::HOLD) sprite0.pos.y += 1.0f;
        if(KML::GetKey(KML_KEY_S) == KML::KeyState::PRESS || KML::GetKey(KML_KEY_S) == KML::KeyState::HOLD) sprite0.pos.y -= 1.0f;
        if(KML::GetKey(KML_KEY_SPACE) == KML::KeyState::PRESS) {
            KML::Event(KML::WindowEvent::FULLSCREEN, 1);
        }
        if(KML::GetKey(KML_KEY_X) == KML::KeyState::PRESS) {
            KML::Event(KML::WindowEvent::FULLSCREEN, 0);
        }

        sprite0.Draw();
        //
        int w, h;
        KML::GetWindowSize(&w, &h);
        KML::SetWindowViewport(w, h);

        KML::DrawFramebuffer(framebuff, framebufferProgram);
        KML::SwapBuffers();
        //KML::PresentFrame(KML::Vec3f(0.5f));
    }
    KML::DeleteFramebuffer(framebuff);
    KML::DeleteShape(shape0);
    texManager.Clear();
    shaderManager.Clear();
    KML::Quit();
    return 0;
}