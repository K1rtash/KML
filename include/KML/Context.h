#ifndef KML_CONTEXT_H
#define KML_CONTEXT_H

#include "ABI.h"

/**
 * @file       Context.h 
 * @brief      Window creation and event processing
 */

/**
 * @namespace KML 
 * @brief Contains all public methods, classes, enums, and structs of the library
 */
namespace KML {
    /**
     * @brief      Flags for both mouse and keyboard buttons
     */
    enum class KeyState{PRESS, RELEASE, HOLD, UP};

    /**
     * @brief      Defines the name of a window event, needs a flag
     */
    enum class WindowEvent{EXIT, FULLSCREEN, ICONIFY, HIDE, MAXIMIZE, MOUSE_CAPTURED};

    /**
     * @brief      Flags used to create the graphical context
     */
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

    /**
     * @brief      Sets the targeted GL context
     *
     *             Must be called before a window is initialized
     *
     * @param[in]  major  Major version
     * @param[in]  minor  Minor version
     */
    KML_API void SetGLContextVersion(int major, int minor);

    /**
     * @brief      Creates a unique graphical context for this library
     *
     * @param[in]  width   Window width
     * @param[in]  height  Window height
     * @param[in]  title   Window title
     * @param[in]  flags   Optional window flags flags
     *
     * @return     True on success, false on failure
     */
    KML_API bool InitWindow(int width, int height, const char* title, unsigned int flags = NONE);

    /**
     * @brief      Process keystrokes, mouse input, and user events
     *
     *             Must be called before every frame
     *
     * @return     Will return false on close event
     */
    KML_API bool ProcessEvents();

    /**
     * @brief      Gets the mouse capture state.
     *
     * @return     True if the mouse is captured, false if the mouse is free
     */
    KML_API bool GetMouseCaptureState();

    /**
     * @brief      Frees all resources used in the library
     *             You must not use any method aftern calling this
     */
    KML_API void Quit();

    /**
     * @brief          Controls the window state using events
     * @param event    Flag
     * @param value    Value
     */
    KML_API void Event(WindowEvent event, int value);
    
    /**
     * @brief      Clears the screen with the given color and draws the current frame
     *
     * @param[in]  r     Red color
     * @param[in]  g     Green color
     * @param[in]  b     Blue color
     */
    KML_API void PresentFrame(float r = 0.0f, float g = 0.0f, float b = 0.0f, bool TEMP = true);

    KML_API void getScreenMeasure(int* w, int* h);
    KML_API void setLogicalPresentation(int width, int height);

    /**
     * @brief      Prints the current OpenGL context to stdout
     */
    KML_API void PrintContext();

    /**
     * @brief            Gets the key's state as enum
     * 
     *                   The state is registered when processing events
     *
     * @param[in]  key   Key id 
     *
     * @return           @see KeyState
     */
    KML_API KeyState GetKey(int key);

    /**
     * @brief            Gets the mouse button state as enum
     * 
     *                   The state is registered when processing events
     *
     * @param[in]  key   Mouse button id 
     *
     * @return           @see KeyState
     */
    KML_API KeyState GetMouseButton(int button);

    /**
     * @brief      Sets the window title
     *
     * @param[in]  title  Title string
     */
    KML_API void SetWindowTitle(const char* title);

    /**
     * @brief      Sets the window icon.
     * 
     * Windows: if the window is decorated it will set the icon of the title bar
     * Linux: On cinnamon/LXQt desktops this will set the taskbar icon, not the window decoration
     *
     * @param[in]  file  Image file in disc
     */
    KML_API void SetWindowIcon(const char* file);

    KML_API void UseFramebuffer(unsigned int framebuffer);
}
#endif