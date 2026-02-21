#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

#include <iostream>
#include <cassert>

#include "KML/Context.h"
#include "KML/Keycodes.h"

#include "__KML/graphics.h"

/* ---------- Declaraciones de callbacks ---------- */

void error_callback(int, const char*);
void resize_callback(GLFWwindow*, int, int);
void scroll_callback(GLFWwindow*, double, double);
void cursor_callback(GLFWwindow*, double, double);
void window_focus_callback(GLFWwindow*, int);
void key_callback(GLFWwindow*, int, int, int, int);
void mouse_button_callback(GLFWwindow*, int, int, int);
void iconify_callback(GLFWwindow* window, int iconified);

/* ---------- Declaraciones propias ---------- */

void setGLcontext(int& major, int& minor, bool useLatestCtx);
void printGLInfo(bool);
void updateKeyboard();

struct Window{
    int width, height;
    bool focused = true;
    const char* title = "KML Window";
    GLFWwindow* handle = nullptr;
} window;

struct Input {
    double scrollDelta = 0.0;
    float scrollDX = 0.0f, scrollDY = 0.0f;
    bool mouseCaptured = false;
    bool rawButtons[GLFW_KEY_LAST]{false};
    KML::KeyState buttons[GLFW_KEY_LAST]{KML::KeyState::UP};
} input;

namespace __KML {
    float LOG_SCREEN_WIDTH = 1080.0f, LOG_SCREEN_HEIGHT = 720.0f, LOG_SCREEN_ASPECT = LOG_SCREEN_WIDTH / LOG_SCREEN_HEIGHT;
    int glContext[2] = {3,3};
    int contextActive = 0;
}

/* ---------- Definiciones ---------- */
void KML::SetGLContextVersion(int major, int minor) {
    __KML::glContext[0] = major;
    __KML::glContext[1] = minor;
}
bool KML::InitWindow(int width, int height, const char* title, unsigned int flags) {
    __KML::LOG_SCREEN_WIDTH = width;
    __KML::LOG_SCREEN_HEIGHT = height;
    __KML::LOG_SCREEN_ASPECT = __KML::LOG_SCREEN_WIDTH / __KML::LOG_SCREEN_HEIGHT;
    glfwSetErrorCallback(error_callback);
    glfwInit();
        
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* video = glfwGetVideoMode(monitor);
    
    setGLcontext(__KML::glContext[0], __KML::glContext[1], (flags & KML::GL_CONTEXT_LATEST) ? true : false);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, __KML::glContext[0]);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, __KML::glContext[1]);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if(flags & KML::ENABLE_VSYNC) glfwWindowHint(GLFW_REFRESH_RATE, video->refreshRate);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    int aasamples =
        (flags & KML::MSAA8) ? 8 :
        (flags & KML::MSAA4) ? 4 :
        (flags & KML::MSAA2) ? 2 : 0;
    
    if(aasamples > 0) glfwWindowHint(GLFW_SAMPLES, aasamples);
    glfwWindowHint(GLFW_DECORATED, (flags & KML::BORDERLESS) ? 0 : 1);
    glfwWindowHint(GLFW_RESIZABLE, (flags & KML::RESIZABLE) ? 1 : 0);

    if(flags & KML::MAXIMIZED && flags & KML::BORDERLESS && !(flags & KML::FULLSCREEN)) 
    {
        int x, y, w, h;
        glfwGetMonitorWorkarea(monitor, &x, &y, &w, &h);
        window.handle = glfwCreateWindow(w, h, title, nullptr, nullptr);
        glfwSetWindowPos(window.handle, x, y);
    }
    else if(flags & KML::MAXIMIZED && !(flags & KML::BORDERLESS) && !(flags & KML::FULLSCREEN)) 
    {
        window.handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
        glfwMaximizeWindow(window.handle);
    }
    else if(flags & KML::FULLSCREEN) 
    {
        glfwWindowHint(GLFW_RED_BITS, video->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, video->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, video->blueBits);
        int w, h;
        w = video->width;  // Use our 'desktop' resolution for window size
        h = video->height; // to get a 'full screen borderless' window.
        window.handle = glfwCreateWindow(w, h, title, monitor, nullptr);
    }
    else window.handle = glfwCreateWindow(width, height, title, nullptr, nullptr);

    glfwMakeContextCurrent(window.handle);
    glfwGetWindowSize(window.handle, &window.width, &window.height);
    window.title = title;

    if(flags & KML::LOCK_ASPECT) glfwSetWindowAspectRatio(window.handle, width, height);

    if(glfwRawMouseMotionSupported())
        glfwSetInputMode(window.handle, GLFW_RAW_MOUSE_MOTION, 1);
    else 
        std::cout << "Raw mouse imput is not supported on this device!\n";

    glfwSetWindowSizeCallback(window.handle, resize_callback);
    glfwSetScrollCallback(window.handle, scroll_callback);
    glfwSetCursorPosCallback(window.handle, cursor_callback);
    glfwSetWindowFocusCallback(window.handle, window_focus_callback);
    glfwSetKeyCallback(window.handle, key_callback);
    glfwSetMouseButtonCallback(window.handle, mouse_button_callback);
    glfwSetWindowIconifyCallback(window.handle, iconify_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return false;
    if(flags & KML::ENABLE_VSYNC) glfwSwapInterval(1);

    printGLInfo(false);

    if(aasamples > 0) glEnable(GL_MULTISAMPLE); 
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glDepthFunc(GL_LESS);
    //glCullFace(GL_FRONT);
    //glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    __KML::GenerateDefaultMembers();
    __KML::InitAudioDevice();
    __KML::InitFreeType();

    __KML::contextActive = 1;
    return window.handle;
}

void KML::Quit() {
    if(!__KML::contextActive) return;
    __KML::CloseAudioDevice();
    __KML::QuitFreeType(); 
    DeleteShader(__KML::defaultShader);
    DeleteShape(__KML::defaultShape);
    glfwDestroyWindow(window.handle);
    glfwTerminate();
    __KML::contextActive = 0;
}

void KML::Event(WindowEvent event, int value) {
    static int windowedX = 0, windowedY = 0, windowedWidth = 0, windowedHeight = 0;
    switch(event) {
    case WindowEvent::EXIT: 
        glfwSetWindowShouldClose(window.handle, value);
        break;
    case WindowEvent::ICONIFY:
        if(value) glfwIconifyWindow(window.handle);        
        else glfwRestoreWindow(window.handle);
        break;
    case WindowEvent::MOUSE_CAPTURED:
        if(value) glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        input.mouseCaptured = value;
        break;
    case WindowEvent::HIDE:
        if(value) glfwHideWindow(window.handle);        
        else glfwShowWindow(window.handle);
        break;
    case WindowEvent::FULLSCREEN: {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        if (value) {
            glfwGetWindowPos(window.handle, &windowedX, &windowedY);
            glfwGetWindowSize(window.handle, &windowedWidth, &windowedHeight);

            glfwSetWindowMonitor(window.handle, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        } else {
            if(windowedWidth == 0) windowedWidth = __KML::LOG_SCREEN_WIDTH;
            if(windowedHeight == 0) windowedHeight = __KML::LOG_SCREEN_HEIGHT;
            glfwSetWindowMonitor(window.handle, nullptr, windowedX, windowedY, windowedWidth, windowedHeight, 0);
        }
        break;
    }
    case WindowEvent::MAXIMIZE:
        if(value) glfwMaximizeWindow(window.handle);
        else glfwRestoreWindow(window.handle);
        break;
    case WindowEvent::HIGHLIGHT:
        if(value) glfwRequestWindowAttention(window.handle);
        break;
    }
}

bool KML::ProcessEvents() {
    glfwPollEvents();
    updateKeyboard();

    return !glfwWindowShouldClose(window.handle);
}

void KML::SetWindowViewport(int width, int height) {
    float aspect = (float)width / (float)height;
    int viewportX, viewportY, viewportW, viewportH;

    if (aspect > __KML::LOG_SCREEN_ASPECT) {
        viewportH = height;
        viewportW = (int)(height * __KML::LOG_SCREEN_ASPECT);
        viewportX = (width - viewportW) / 2;
        viewportY = 0;
    } else {
        viewportW = width;
        viewportH = (int)(width / __KML::LOG_SCREEN_ASPECT);
        viewportX = 0;
        viewportY = (height - viewportH) / 2;
    }

    glViewport(viewportX, viewportY, viewportW, viewportH);
}

void KML::GetWindowSize(int* w, int* h) {
    assert(w && h);
    *w = window.width;
    *h = window.height;
}

void error_callback(int error, const char* description) {
    printf("GLFW error: %s\n", description);
}

void resize_callback(GLFWwindow* handle, int width, int height) {
    window.width = width;
    window.height = height;
}

void scroll_callback(GLFWwindow* handle, double xoffset, double yoffset) {
    input.scrollDelta += yoffset;
}

void cursor_callback(GLFWwindow* handle, double xpos, double ypos) {
    static bool first = true;
    static double lastX, lastY;

    if(first) {
        lastX = xpos;
        lastY = ypos;
        first = false;
        return;
    }

    input.scrollDX += float(xpos - lastX);
    input.scrollDY += float(ypos - lastY);

    lastX = xpos;
    lastY = ypos;
}

void window_focus_callback(GLFWwindow* handle, int focused) {
    if(focused) {
        window.focused = true;
        /* Si el ratón estaba capturado, devolverlo al modo */
        if(input.mouseCaptured) glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
    }
    else {
        window.focused = false;
        glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void key_callback(GLFWwindow* handle, int key, int scancode, int action, int mods) {
    if (key < GLFW_KEY_SPACE || key >= GLFW_KEY_LAST) return;

    if (action == GLFW_PRESS) 
        input.rawButtons[key] = true;
    else if (action == GLFW_RELEASE) 
        input.rawButtons[key] = false;
}

void mouse_button_callback(GLFWwindow* handle, int button, int action, int mods) {
    if(button < 0 || button >= GLFW_MOUSE_BUTTON_LAST) return;
    input.rawButtons[button] = action;
}

void iconify_callback(GLFWwindow* handle, int iconified)
{
    if (iconified) {
        window.focused = false;
        glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else {
        window.focused = true;
        if(input.mouseCaptured) glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

KML::KeyState KML::GetKey(int key) {
    assert(key >= GLFW_KEY_SPACE && key <= GLFW_KEY_LAST);
    return input.buttons[key];
}

KML::KeyState KML::GetMouseButton(int button) {
    assert(button >= 0 && button <= GLFW_MOUSE_BUTTON_LAST);
    return input.buttons[button];
}

KML::KeyState setButtonState(int i, bool isDown) {
    switch (input.buttons[i]) {
        case KML::KeyState::UP: return isDown ? KML::KeyState::PRESS : KML::KeyState::UP;
        case KML::KeyState::PRESS: return isDown ? KML::KeyState::HOLD : KML::KeyState::RELEASE;
        case KML::KeyState::HOLD: return isDown ? KML::KeyState::HOLD : KML::KeyState::RELEASE;
        case KML::KeyState::RELEASE: return isDown ? KML::KeyState::PRESS : KML::KeyState::UP;
    }   
    return KML::KeyState::UP;
}

void updateKeyboard() {
    for(int i = GLFW_MOUSE_BUTTON_1; i < GLFW_KEY_SPACE; i++)
        input.buttons[i] = setButtonState(i, input.rawButtons[i]);
    for (int i = GLFW_KEY_SPACE; i < GLFW_KEY_LAST; i++)
        input.buttons[i] = setButtonState(i, input.rawButtons[i]);
}

bool KML::GetMouseCaptureState() {
    return input.mouseCaptured;
}

void KML::SwapBuffers() {
    glfwSwapBuffers(window.handle);
}

void KML::PresentFrame(Vec3f c) {
    glfwSwapBuffers(window.handle);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(c.x, c.y, c.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SetWindowViewport(window.width, window.height);
}

bool tryGLcontext(int major, int minor) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* th = glfwCreateWindow(1, 1, "test", nullptr, nullptr);
    bool valid = th;
    glfwDestroyWindow(th);

    std::cout << "[INFO] Tried GL version: " << major << "." << minor << (valid ? " (SUPPORTED)" : " (UNSUPORTED)") << std::endl;
    return valid;
}

void setGLcontext(int& major, int& minor, bool useLatestCtx) {
    if(tryGLcontext(major, minor) && !useLatestCtx) return;

    int availible[][2] = { {4,6}, {4,5}, {4,3}, {4,1}, {3,3} };

    for(int i = 0; i < sizeof(availible)/sizeof(int[2]); i++) {
        if(tryGLcontext(availible[i][0], availible[i][1])) {
            major = availible[i][0];
            minor = availible[i][1];
            break;
        }
    }

    if(major == 0 && minor == 0) throw std::runtime_error("Device drivers cant support minimum required GL version");
}

void KML::PrintContext() {
    printf("Screen aspect: %dx%d\nLogical aspect: %fx%f\n", window.width, window.height, __KML::LOG_SCREEN_WIDTH, __KML::LOG_SCREEN_HEIGHT);
}

void KML::SetWindowTitle(const char* t) {
    if(t) glfwSetWindowTitle(window.handle, t);
}

void KML::SetWindowIcon(const char* file) {
    GLFWimage img;
    int cc;
    img.pixels = stbi_load(file, &img.width, &img.height, &cc, 0);

    const GLFWimage* iptr = &img;
    assert(iptr->pixels);
    glfwSetWindowIcon(window.handle, 1, iptr);    
}

bool KML::GetWindowFocused() {
    return window.focused;
}

void printGLInfo(bool showExt) /// Se debe llamar siempre despues de crear el contexto OpenGL makeContextCurrent
{ 
    const GLubyte* renderer = glGetString(GL_RENDERER); // GPU
    const GLubyte* vendor   = glGetString(GL_VENDOR);   // fabricante
    const GLubyte* version  = glGetString(GL_VERSION);  // versión OpenGL
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION); // GLSL
    int profile, ext_n = 0, ext_i;
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
    glGetIntegerv(GL_NUM_EXTENSIONS, &ext_n);
    glGetStringi(GL_EXTENSIONS, ext_i);

    std::cout << "[INFO] Printing machine specifications: \n"
                << "* GPU: " << reinterpret_cast<const char*>(renderer) << "\n" 
                << "* Vendor: " << reinterpret_cast<const char*>(vendor) << "\n"
                << "* OpenGL version: " << reinterpret_cast<const char*>(version) << " " << ((profile & GL_CONTEXT_CORE_PROFILE_BIT) ? "Core" : "Compatibility") << "\n"
                << "* GLSL version: " << reinterpret_cast<const char*>(glslVersion) << "\n"
                << "* Extensions";

    if(showExt) {
        std::cout << ": (" << ext_n << ")\n";
        for (int i = 0; i < ext_n; ++i) {
            const char* ext = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
            std::cout << "  - " << ext << "\n";
        }
    } else std::cout << ": " << ext_n << '\n';
}