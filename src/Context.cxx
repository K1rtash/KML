#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Contex.h"

/* ---------- Declaraciones de callbacks ---------- */

void error_callback(int, const char*);
void resize_callback(GLFWwindow*, int, int);
void scroll_callback(GLFWwindow*, double, double);
void cursor_callback(GLFWwindow*, double, double);
void window_focus_callback(GLFWwindow*, int);
void key_callback(GLFWwindow*, int, int, int, int);
void mouse_button_callback(GLFWwindow*, int, int, int);

/* ---------- Declaraciones propias ---------- */

void setLogicalPresentation(int, int);
void printGLInfo();
void updateKeyboard();

struct WindowCtx{
    const int LOG_WIDTH = 1080, LOG_HEIGHT = 720;
    const float LOG_ASPECT = (float)LOG_WIDTH / (float)LOG_HEIGHT;
    int width, height;
    bool focused = true;
    const char* title = "KML Window";
    KML::WindowMode mode = KML::WindowMode::MAXIMIZED_WINDOWED;
    GLFWwindow* handle = nullptr;
} window;

struct Input {
    double scrollDelta = 0.0;
    float scrollDX = 0.0f, scrollDY = 0.0f;
    bool mouseCaptured = false;
    //bool keyboard[GLFW_KEY_LAST]{false};
    bool rawButtons[GLFW_KEY_LAST]{false};
    KML::KeyState buttons[GLFW_KEY_LAST]{KML::KeyState::UP};
    //KML::KeyState buttons[GLFW_MOUSE_BUTTON_LAST]{KML::KeyState::UP};
} input;

/* ---------- Definiciones ---------- */

bool KML::Init(int width, int height, const char* title, WindowFlags flags) {
    glfwSetErrorCallback(error_callback);
    glfwInit();
        
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* video = glfwGetVideoMode(monitor);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_REFRESH_RATE, video->refreshRate);
    glfwWindowHint(GLFW_SAMPLES, 8);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    
    switch (window.mode) {
        case KML::WindowMode::FULLSCREEN: {
            glfwWindowHint(GLFW_RED_BITS, video->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, video->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, video->blueBits);
            int w, h;
            w = video->width;  // Use our 'desktop' resolution for window size
            h = video->height; // to get a 'full screen borderless' window.
            window.handle = glfwCreateWindow(w, h, title, monitor, nullptr);
            break;
        }
        case KML::WindowMode::MAXIMIZED_BORDERLESS: {
            glfwWindowHint(GLFW_DECORATED, false);
            int x, y, w, h;
            glfwGetMonitorWorkarea(monitor, &x, &y, &w, &h);
            window.handle = glfwCreateWindow(w, h, title, nullptr, nullptr);
            glfwSetWindowPos(window.handle, x, y);
            break;
        }
        case KML::WindowMode::MAXIMIZED_WINDOWED: {
            window.handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
            glfwMaximizeWindow(window.handle);
            break;
        }
        default:
            glfwWindowHint(GLFW_RESIZABLE, true);
            window.handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
    }

    if(window.handle == nullptr) return false;

    glfwMakeContextCurrent(window.handle);
    glfwGetWindowSize(window.handle, &window.width, &window.height);
    window.title = title;

    if (glfwRawMouseMotionSupported()) 
        glfwSetInputMode(window.handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    else 
        std::cerr << "[WARNING] This machine does not support raw mouse input\n";

    glfwSetWindowSizeCallback(window.handle, resize_callback);
    glfwSetScrollCallback(window.handle, scroll_callback);
    glfwSetCursorPosCallback(window.handle, cursor_callback);
    glfwSetWindowFocusCallback(window.handle, window_focus_callback);
    glfwSetKeyCallback(window.handle, key_callback);
    glfwSetMouseButtonCallback(window.handle, mouse_button_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return false;
    glfwSwapInterval(false);

    setLogicalPresentation(window.width, window.height);
    printGLInfo();

    glEnable(GL_MULTISAMPLE); 
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glDepthFunc(GL_LESS);
    //glCullFace(GL_FRONT);
    //glFrontFace(GL_CCW);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return true;
}

void KML::Terminate() {
    glfwDestroyWindow(window.handle);
    glfwTerminate();
}

void KML::Close() {
    glfwSetWindowShouldClose(window.handle, true);
}

bool KML::ProcessEvents() {
    glfwPollEvents();
    updateKeyboard();

    return !glfwWindowShouldClose(window.handle);
}

void setLogicalPresentation(int width, int height) {
    window.width = width;
    window.height = height;

    float aspect = (float)width / (float)height;
    int viewportX, viewportY, viewportW, viewportH;

    if (aspect > window.LOG_ASPECT) {
        viewportH = height;
        viewportW = (int)(height * window.LOG_ASPECT);
        viewportX = (width - viewportW) / 2;
        viewportY = 0;
    } else {
        viewportW = width;
        viewportH = (int)(width / window.LOG_ASPECT);
        viewportX = 0;
        viewportY = (height - viewportH) / 2;
    }

    glViewport(viewportX, viewportY, viewportW, viewportH);
}


void printGLInfo() { 
    const GLubyte* renderer = glGetString(GL_RENDERER); // GPU
    const GLubyte* vendor   = glGetString(GL_VENDOR);   // fabricante
    const GLubyte* version  = glGetString(GL_VERSION);  // versión OpenGL
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION); // GLSL

    std::cout << "[INFO] Printing machine specifications: \n"
                << "* GPU: " << reinterpret_cast<const char*>(renderer) << "\n" 
                << "* Vendor: " << reinterpret_cast<const char*>(vendor) << "\n"
                << "* OpenGL version supported: " << reinterpret_cast<const char*>(version) << "\n"
                << "* GLSL version supported: " << reinterpret_cast<const char*>(glslVersion) << "\n";
}

void error_callback(int error, const char* description) {
    std::cerr << "[GLFW] Error ( " << error << " ) " << description << "\n"; 
}

void resize_callback(GLFWwindow* handle, int width, int height) {
    setLogicalPresentation(width, height);
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
        std::cout << "[DEBUG] Window is focused\n";
    }
    else {
        window.focused = false;
        /* Opcional: el ratón deja de estar capturado */
        //input.mouseCaptured = 0;
        glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        std::cout << "[DEBUG] Window lost focus\n";
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

KML::KeyState KML::GetKey(int key) {
    if(key < GLFW_KEY_SPACE || key > GLFW_KEY_LAST) throw std::invalid_argument("in KML::KeyState KML::GetKey(int key) -> value out of bounds");
    return input.buttons[key];
}

KML::KeyState KML::GetMouseButton(int button) {
    if(button < 0 || button > GLFW_MOUSE_BUTTON_LAST) throw std::invalid_argument("in KML::KeyState KML::GetMouseButton(int button) -> value out of bounds");
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

void KML::SetMouseCaptured(bool captured) {
    if(captured) glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    input.mouseCaptured = captured;
}

bool KML::GetMouseCaptureState() {
    return input.mouseCaptured;
}

void KML::PresentFrame(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glfwSwapBuffers(window.handle);
}