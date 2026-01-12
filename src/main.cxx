#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

/* ---------- Datos estáticos ---------- */

enum WindowMode{FULLSCREEN, MAXIMIZED_BORDERLESS, MAXIMIZED_WINDOWED};
enum KeyState{PRESS, RELEASE, HOLD, UP};

int glCtxMajor = 3, glCtxMinor = 3, resizable = 0;

struct WindowCtx{
    const int LOG_WIDTH = 1080, LOG_HEIGHT = 720, LOG_ASPECT = LOG_WIDTH / LOG_HEIGHT;
    int width, height;
    bool focused;
    const char* title = "KML Window";
    WindowMode mode = MAXIMIZED_WINDOWED;
} winCtx;

struct Input {
    double scrollDelta = 0.0;
    float scrollDX = 0.0f, scrollDY = 0.0f;
    bool mouseCaptured = true;
    bool keyboard[GLFW_KEY_LAST]{false};
    KeyState keys[GLFW_KEY_LAST]{UP};
} input;

/* ---------- Declaraciones propias ---------- */

void setLogicalPresentation(int width, int height); 
void printGLInfo();
void updateKeyboard();
KeyState getKey(int key);

/* ---------- Declaraciones de callbacks de GLFW ---------- */

void error_callback(int error, const char* description);
void resize_callback(GLFWwindow *window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);
void window_focus_callback(GLFWwindow* window, int focused);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

/* ---------- Main ---------- */

int main(void)
{
    glfwInit();
        
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* video = glfwGetVideoMode(monitor);
    GLFWwindow* window;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glCtxMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glCtxMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_REFRESH_RATE, video->refreshRate);
    glfwWindowHint(GLFW_SAMPLES, 8);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    switch (winCtx.mode) {
        case FULLSCREEN: {
            glfwWindowHint(GLFW_RED_BITS, video->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, video->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, video->blueBits);
            int w, h;
            w = video->width;  // Use our 'desktop' resolution for window size
            h = video->height; // to get a 'full screen borderless' window.
            window = glfwCreateWindow(w, h, winCtx.title, monitor, nullptr);
            break;
        }
        case MAXIMIZED_BORDERLESS: {
            glfwWindowHint(GLFW_DECORATED, false);
            int x, y, w, h;
            glfwGetMonitorWorkarea(monitor, &x, &y, &w, &h);
            window = glfwCreateWindow(w, h, winCtx.title, nullptr, nullptr);
            glfwSetWindowPos(window, x, y);
            break;
        }
        case MAXIMIZED_WINDOWED: {
            window = glfwCreateWindow(winCtx.LOG_WIDTH, winCtx.LOG_WIDTH, winCtx.title, nullptr, nullptr);
            glfwMaximizeWindow(window);
            break;
        }
        default:
            glfwWindowHint(GLFW_RESIZABLE, resizable);
            window = glfwCreateWindow(winCtx.LOG_WIDTH, winCtx.LOG_WIDTH, winCtx.title, nullptr, nullptr);
    }

    glfwMakeContextCurrent(window);
    glfwGetWindowSize(window, &winCtx.width, &winCtx.height);

    if (glfwRawMouseMotionSupported()) 
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    else 
        std::cerr << "[WARNING] This machine does not support raw mouse input\n";

    glfwSetErrorCallback(error_callback);
    glfwSetWindowSizeCallback(window, resize_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetWindowFocusCallback(window, window_focus_callback);
    glfwSetKeyCallback(window, key_callback);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(false);
    setLogicalPresentation(winCtx.width, winCtx.height);

    printGLInfo();

    glEnable(GL_MULTISAMPLE); 
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glDepthFunc(GL_LESS);
    //glCullFace(GL_FRONT);
    //glFrontFace(GL_CCW);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        updateKeyboard();

        if(getKey(GLFW_KEY_ESCAPE) == PRESS || getKey(GLFW_KEY_ESCAPE) == HOLD) {
            glfwSetWindowShouldClose(window, true);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

/* ---------- Declaraciones propias ---------- */

void updateKeyboard() {
    for (int i = GLFW_KEY_SPACE; i < GLFW_KEY_LAST; i++) {
        bool isDown = input.keyboard[i];

        /* Según como estaban en el frame anterior, le asignamos un nuevo estado */
        switch (input.keys[i]) {
            case UP: if(isDown) input.keys[i] = PRESS; break;
            case PRESS: input.keys[i] = isDown ? HOLD : RELEASE; break;
            case HOLD: if(!isDown) input.keys[i] = RELEASE; break;
            case RELEASE: input.keys[i] = isDown ? PRESS : UP; break;
        }     
    }
}

KeyState getKey(int key) {
    return input.keys[key];
}

void setLogicalPresentation(int width, int height) {
    winCtx.width = width;
    winCtx.height = height;

    float aspect = (float)width / (float)height;
    int viewportX, viewportY, viewportW, viewportH;

    if (aspect > winCtx.LOG_ASPECT) {
        viewportH = height;
        viewportW = (int)(height * winCtx.LOG_ASPECT);
        viewportX = (width - viewportW) / 2;
        viewportY = 0;
    } else {
        viewportW = width;
        viewportH = (int)(width / winCtx.LOG_ASPECT);
        viewportX = 0;
        viewportY = (height - viewportH) / 2;
    }

    glViewport(viewportX, viewportY, viewportW, viewportH);
}

void printGLInfo() /// Se debe llamar siempre despues de crear el contexto OpenGL makeContextCurrent
{ 
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

/* ---------- Definiciones de callbacks de GLFW ---------- */

void error_callback(int error, const char* description) {
    std::cerr << "[GLFW] Error ( " << error << " ) " << description << "\n"; 
}

void resize_callback(GLFWwindow *window, int width, int height) {
    if(winCtx.mode == MAXIMIZED_WINDOWED) {
        glfwMaximizeWindow(window);
        return;
    }
    setLogicalPresentation(width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    input.scrollDelta += yoffset;
}

void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
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

void window_focus_callback(GLFWwindow* window, int focused) {
    if(focused) {
        winCtx.focused = true;
        /* Si el ratón estaba capturado, devolverlo al modo */
        if(input.mouseCaptured) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        std::cout << "[DEBUG] Window is focused\n";
    }
    else {
        winCtx.focused = false;
        /* Opcional: el ratón deja de estar capturado */
        //input.mouseCaptured = 0;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        std::cout << "[DEBUG] Window lost focus\n";
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key < GLFW_KEY_SPACE || key >= GLFW_KEY_LAST) return;

    if (action == GLFW_PRESS) 
        input.keyboard[key] = true;
    else if (action == GLFW_RELEASE) 
        input.keyboard[key] = false;
}