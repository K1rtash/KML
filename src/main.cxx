#include "KML/kml.h"
#include "KML/Texture.h"
#include <iostream>

int main(void) {
    KML::Init(800, 600, "KML Window", KML::RESIZABLE | KML::ENABLE_VSYNC | KML::MSAA4 | KML::GL_CONTEXT_LATEST);

    load("images.png");
    unsigned int id = getID("images.png");

    while(KML::ProcessEvents()) {
        if(KML::GetKey(KML_KEY_ESCAPE) == KML::KeyState::PRESS) {
            if (!KML::GetMouseCaptureState())
                KML::Close();
            else 
                KML::SetMouseCaptured(false);
        }

        if(KML::GetMouseButton(KML_MOUSE_BUTTON_LEFT) == KML::KeyState::PRESS) 
            KML::SetMouseCaptured(true);

        bind(id);
        KML::PresentFrame(0.2f, 0.3f, 0.3f, 1.0f);
    }

    KML::Terminate();
    return 0;
}