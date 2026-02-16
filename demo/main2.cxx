#include <iostream>
#include <KML/KML.h>
#include <glad/glad.h>

int width = 320, height = 180;

int main() {
    KML::InitWindow(width, height, "Conditioned Subject", KML::RESIZABLE | KML::ENABLE_VSYNC | KML::MSAA8 | KML::GL_CONTEXT_LATEST);

    KML::Clock clock0{};

    KML::Shader* shader0 = KML::CreateShader("assets/shaders/logo.vs", "assets/shaders/logo.fs");
    KML::Shader* framebufferProgram = KML::CreateShader("assets/shaders/fb.vs", "assets/shaders/fb.fs");
    KML::SetUniform_1i("screenTexture", framebufferProgram, 0);

    KML::Texture tex0 = KML::LoadTexture("assets/logo.png");

    float quad[] = {
     //  x     y     z      u     v
       -1.f, -1.f, 0.f,   0.f, 0.f,
        1.f, -1.f, 0.f,   1.f, 0.f,
        1.f,  1.f, 0.f,   1.f, 1.f,
       -1.f,  1.f, 0.f,   0.f, 1.f
    };

    unsigned int quadIdx[] = {
        0, 1, 2,
        2, 3, 0
    };

    KML::Shape shape0{quad, sizeof(quad), quadIdx, sizeof(quadIdx)};

    float u = 1.0f - (1.0f / 320.0f);
    float v = 1.0f - (1.0f / 180.0f);

    float rectangleVertices[] =
    {
         1.0f, -1.0f,  u, 0.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, v,

         1.0f,  1.0f,  u, v,
         1.0f, -1.0f,  u, 0.0f,
        -1.0f,  1.0f,  0.0f, v
    };


    unsigned int rectVAO, rectVBO;
    glGenVertexArrays(1, &rectVAO);
    glGenBuffers(1, &rectVBO);
    glBindVertexArray(rectVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // Create Frame Buffer Object
    unsigned int FBO;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Create Framebuffer Texture
    unsigned int framebufferTexture;
    glGenTextures(1, &framebufferTexture);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

    // Create Render Buffer Object
    unsigned int RBO;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


    // Error checking framebuffer
    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer error: " << fboStatus << std::endl;



    while(KML::ProcessEvents()) {
        float time = (float)clock0.Now();

        KML::SetUniform_1f("u_time", shader0, time);
        KML::SetUniform_2fv("u_resolution", shader0, KML::Vec2f((float)width, (float)height));
        KML::SetUniform_3fv("u_logoColor", shader0, KML::Vec3f(1.0f, 0.0f, 0.0f));

        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glViewport(0, 0, width, height);
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // Drawing

        KML::UseShader(shader0);
        KML::BindTexture(tex0, 0);
        //shape0.Draw();

        if(KML::GetKey(KML_KEY_ESCAPE) == KML::KeyState::PRESS) {
            KML::Event(KML::WindowEvent::EXIT, 1);
        }

        if(KML::GetKey(KML_KEY_R) == KML::KeyState::PRESS) {
            KML::ReloadShader(shader0);
        }

        //
        int w, h;
        KML::getScreenMeasure(&w, &h);
        KML::setLogicalPresentation(w, h);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1,1,1,1);
        glClear(GL_COLOR_BUFFER_BIT);
        KML::UseShader(framebufferProgram);
        glBindVertexArray(rectVAO);
        glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
        glBindTexture(GL_TEXTURE_2D, framebufferTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        KML::PresentFrame(1.0f, 1.0f, 1.0f, true);
    }

    KML::Quit();
    return 0;
}