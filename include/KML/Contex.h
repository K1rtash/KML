#ifndef KML_CONTEXT_H
#define KML_CONTEXT_H

namespace KML {
    enum class KeyState{PRESS, RELEASE, HOLD, UP};
    enum WindowFlags : unsigned int {
        NONE = 0,
        RESIZABLE = 1 << 0,
        ENABLE_VSYNC = 1 << 1,
        MSAA2 = 1 << 2,
        MSAA4 = 1 << 3,
        MSAA8 = 1 << 4,
        MAXIMIZED = 1 << 5,
        BORDERLESS = 1 << 6,
        FULLSCREEN = 1 << 7,
        GL_CONTEXT_LATEST = 1 << 8
    };

    bool CreateWindow(int width, int height, const char* title, unsigned int flags = NONE);
    bool CreateWindowP(int width, int height, const char* title, float logical_width, float logical_height, int glCtxMajor, int glCtxMinor, unsigned int flags);

    bool ProcessEvents();
    bool GetMouseCaptureState();

    void Terminate();
    void Close();
    void PresentFrame(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
    void SetMouseCaptured(bool captured);

    void PrintContext();

    KeyState GetKey(int key);
    KeyState GetMouseButton(int button);
}
#endif