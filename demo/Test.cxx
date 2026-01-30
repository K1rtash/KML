#include "KML/KML.h"

#include <iostream>
#include <string>

#include <fmt/core.h>
#include <fmt/format.h>
#include <ft2build.h>
#include FT_FREETYPE_H


bool keyDown(int k) {
    if(KML::GetKey(k) == KML::KeyState::PRESS || KML::GetKey(k) == KML::KeyState::HOLD) return true;
    return false;
}

int main(void) {
    std::cout << "cwd: " << std::filesystem::current_path() << "\n";
    
    KML::CreateWindow(800, 600, "KML Window", KML::RESIZABLE | KML::ENABLE_VSYNC | KML::MSAA8 | KML::GL_CONTEXT_LATEST);

    KML::Surface surface{KML::Vec4f(10.0f, 10.0f, 100.0f, 100.0f)};
    surface.color = KML::HSV_v3f(293, 83, 93);
    surface.anchor = KML::Vec2f(0.5f, 0.5f);

    KML::LoadFont("assets/arial.ttf");
    KML::Shader* shader0 = KML::CreateShader("assets/textv.glsl", "assets/textf.glsl");

    while(KML::ProcessEvents()) {
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

        if(KML::GetMouseButton(KML_MOUSE_BUTTON_LEFT) == KML::KeyState::PRESS) 
            KML::Event(KML::WindowEvent::MOUSE_CAPTURED, 1);


        //surface.Draw();
        KML::RenderText(shader0, "Hola KML!", 25, 300, 1.0f, {1,.5,1});
        KML::PresentFrame();
    }

    KML::Terminate();
    return 0;
}