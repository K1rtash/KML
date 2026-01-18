#include "KML/kml.h"
#include "KML/Layers.h"
#include "KML/SFX.h"

#include <iostream>

bool keyDown(int k) {
    if(KML::GetKey(k) == KML::KeyState::PRESS || KML::GetKey(k) == KML::KeyState::HOLD) return true;
    return false;
}

int gtrnd(int min, int max) {
    return rand () % (max - min + 1) + min;
}

int main(void) {
    srand(time(0));


    KML::CreateWindow(800, 600, "KML Window", KML::RESIZABLE | KML::ENABLE_VSYNC | KML::MSAA8 | KML::GL_CONTEXT_LATEST);
    KML::PrintContext();

    KML::Texture tex0 = KML::LoadTexture("images.png");
    KML::Shader* shader = KML::CreateShader("particulas/vert.glsl", "particulas/frag.glsl"); 

    KML::Surface surface("images.png", {0.0f, 0.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, 0.0f);
    surface.transparency = 20;
    
    KML::Surface srf(KML::Vec4f{30.0f, 50.0f, 70.0f, 1000.0f});
    srf.color = KML::Vec3f(110.0f, 30.0f, 40.0f);

    KML::Surface srf2(KML::Vec4f{100.0f, 100.0f, 500.0f, 500.0f});
    srf2.SetColor_RGB(255, 107, 30);
    srf2.transparency = 30;

    KML::Surface particle{shader};

    KML::Layer layer0;
    layer0.Add(srf);
    layer0.Add(srf2);
    KML::Layer layer1;
    layer1.Add(surface);

    std::vector<KML::Particle> particles;

    KML::Surface background(KML::Vec4f{0.0f, 0.0f, 800.0f, 600.0f});
    background.SetColor_RGB(166, 125, 118);
    std::cout << std::format("BACKGROUNDDDDDD pos: {}, {} scale: {}, {} rot: {}, anchor: {}, {})\n", background.pos.x, background.pos.y, background.scale.x, background.scale.y, background.rotation, background.anchor.x, background.anchor.y); 
    
    while(KML::ProcessEvents()) {
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

        if(KML::GetKey(KML_KEY_Y) == KML::KeyState::PRESS) {
            particles.clear();
            for(int i = 0; i < 10; i++) particles.push_back(KML::GenRandParticle());
        }

        if(KML::GetMouseButton(KML_MOUSE_BUTTON_LEFT) == KML::KeyState::PRESS) 
            KML::SetMouseCaptured(true);

        srf.pos = srf2.pos;

        background.Draw();
        KML::DrawLayers();

        for(int i = 0; i < particles.size(); i++) {
            KML::DrawParticle(shader, 1, particles[i]);
        }

        KML::PresentFrame(0.2f, 0.3f, 0.3f, 1.0f);
    }

    KML::DeleteShader(shader);
    KML::UnloadTexture("images.png");
    KML::Terminate();
    return 0;
}