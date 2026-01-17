#include "KML/kml.h"
#include "KML/Utils.h"
#include <iostream>

bool keyDown(int k) {
    if(KML::GetKey(k) == KML::KeyState::PRESS || KML::GetKey(k) == KML::KeyState::HOLD) return true;
    return false;
}

int main(void) {
    KML::Init(800, 600, "KML Window", KML::RESIZABLE | KML::ENABLE_VSYNC | KML::MSAA8 | KML::GL_CONTEXT_LATEST);

    KML::LoadTexture("images.png");
    KML::Surface surface("images.png", {10.0f, 10.0f, 100.0f, 100.0f}, {0.5f, 0.0f, 0.0f});

    KML::Vec3f hsv = KML::HSV_v3f(28.0f, 1.0f, 1.0f);
    
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
            surface.color.y += 0.1f;
        }
        if(keyDown(KML_KEY_V)) {
            surface.color.y -= 0.1f;
        }

        if(KML::KeyState(KML_KEY_X) == KML::KeyState::PRESS) {
            surface.color = {0.0f, 1.0f, 1.0f};
            surface.rotation = {0.0f, 0.0f, 0.0f};
        }

        if(KML::GetMouseButton(KML_MOUSE_BUTTON_LEFT) == KML::KeyState::PRESS) 
            KML::SetMouseCaptured(true);

        surface.Draw();
        KML::PresentFrame(0.2f, 0.3f, 0.3f, 1.0f);
    }

    KML::Terminate();
    return 0;
}