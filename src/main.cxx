#include "KML/kml.h"
#include "KML/Layers.h"

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

    KML::Texture tex0 = KML::LoadTexture("images.png");

    KML::Surface surface("images.png", {10.0f, 10.0f, 100.0f, 100.0f}, {0.5f, 0.0f, 0.0f});
    KML::Surface srf(KML::Vec2f{30.0f, 50.0f}, KML::Vec2f{70.0f, 1000.0f});
    srf.color = KML::Vec3f(110.0f, 30.0f, 40.0f);

    KML::Surface srf2(KML::Vec2f{1000.0f, 100.0f}, KML::Vec2f{500.0f, 500.0f});
    srf2.SetColor_RGB(255, 107, 30);
    srf2.transparency = 30;
    surface.transparency = 50;

    KML::Shader* shader = KML::CreateShader("vert.glsl", "particles.glsl"); 

    KML::Surface particle{shader};

    KML::Layer layer0;
    layer0.Add(srf);
    layer0.Add(srf2);
    KML::Layer layer1;
    layer1.Add(surface);
    
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
        if(keyDown(KML_KEY_LEFT)) surface.rotation.z += 10.0f;
        if(keyDown(KML_KEY_RIGHT)) surface.rotation.z -= 10.0f;
        if(keyDown(KML_KEY_UP)) surface.scale += KML::Vec2f{5.0f, 5.0f};
        if(keyDown(KML_KEY_DOWN)) surface.scale -= KML::Vec2f{5.0f, 5.0f};
        if(keyDown(KML_KEY_F)) {
            surface.color.y += 10.0f;
            surface.color.x += 5.0f;
        }
        if(keyDown(KML_KEY_V)) {
            surface.color.y -= 10.0f;
            surface.color.x -= 5.0f;
        }

        if(KML::GetKey(KML_KEY_X) == KML::KeyState::PRESS) {
            surface.SetColor_HSV(0);
            surface.rotation = {0.0f, 0.0f, 0.0f};
        }

        if(KML::GetKey(KML_KEY_R) == KML::KeyState::PRESS) { 
            printf("reloading shader!");
            KML::ReloadShader(shader);
        }

        if(KML::GetMouseButton(KML_MOUSE_BUTTON_LEFT) == KML::KeyState::PRESS) 
            KML::SetMouseCaptured(true);

        srf.pos = srf2.pos;
        KML::DrawLayers();
        
        int max_p = 50;
        for(int i = 0; i < max_p; i++) {
            KML::SetUniform_4fv("color", shader, KML::Vec4f{KML::RandFloat(0, 255), KML::RandFloat(0, 255), KML::RandFloat(0, 255), 255.0f});
            KML::SetUniform_1f("radius", shader, KML::RandFloat(0.0f, 1.0f));
            KML::SetUniform_1f("softness", shader, KML::RandFloat(0.0f, 1.0f));
            particle.pos = KML::Vec2f{KML::RandFloat(10.0f, 100.0f), KML::RandFloat(10.0f, 100.0f)};
            particle.scale = KML::Vec2f{KML::RandFloat(100.0f, 1000.0f), KML::RandFloat(100.0f, 1000.0f)};
            particle.Draw();
        }

        KML::PresentFrame(0.2f, 0.3f, 0.3f, 1.0f);
    }

    KML::DeleteShader(shader);
    KML::UnloadTexture("images.png");
    KML::Terminate();
    return 0;
}