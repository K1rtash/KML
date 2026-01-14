#include "Contex.h"
#include "Keycodes.h"
#include <iostream>

int main(void) {
    KML::Init(800, 600, "KML Window", KML::RESIZABLE | KML::ENABLE_VSYNC | KML::MSAA4);

    while(KML::ProcessEvents()) {

        if(KML::GetKey(KML_KEY_ESCAPE) == KML::KeyState::PRESS) {
            if (!KML::GetMouseCaptureState())
                KML::Close();
            else 
                KML::SetMouseCaptured(false);
        }

        if(KML::GetMouseButton(KML_MOUSE_BUTTON_LEFT) == KML::KeyState::PRESS) 
            KML::SetMouseCaptured(true);

        KML::PresentFrame(0.2f, 0.3f, 0.3f, 1.0f);
    }

    KML::Terminate();
    return 0;
}