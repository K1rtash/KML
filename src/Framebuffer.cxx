#include <glad/glad.h>
#include <cassert>
#include <iostream>

#include "KML/Graphics.h"
#include "KML/Vector.h"
#include "__KML/graphics.h"

struct KML::Framebuffer {
	unsigned int FBO, VAO, VBO, RBO, TEX;
	int width, height;
};

KML::Framebuffer* KML::CreateFramebuffer(int width, int height) {
	assert(width > 0 && height > 0);

	Framebuffer* framebuff = new Framebuffer;
	assert(framebuff);

	float vertices[] = {
        // Coords    // texCoords
         1.0f, -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, 1.0f,

         1.0f,  1.0f,  1.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, 1.0f
    };

    unsigned int rectVAO, rectVBO;
    glGenVertexArrays(1, &rectVAO);
    glGenBuffers(1, &rectVBO);
    glBindVertexArray(rectVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
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
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "[ERROR] Creating framebuffer: " << fboStatus << std::endl;
    	return nullptr;
    }

    framebuff->width = width;
    framebuff->height = height;
    framebuff->VAO = rectVAO;
    framebuff->TEX = framebufferTexture;
    framebuff->FBO = FBO;
    framebuff->VBO = rectVBO;
    framebuff->RBO = RBO;
    return framebuff;
}

void KML::DestroyFramebuffer(Framebuffer* fb) {
	assert(fb);

	glDeleteBuffers(1, &(fb->VBO));
	glDeleteFramebuffers(1, &(fb->FBO));
	glDeleteVertexArrays(1, &(fb->VAO));
	glDeleteRenderbuffers(1, &(fb->RBO));
	glDeleteTextures(1, &(fb->TEX));
	delete fb;
}

void KML::BindFramebuffer(Framebuffer* framebuffer, Vec3f col) {
	assert(framebuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->FBO);
    glViewport(0, 0, framebuffer->width, framebuffer->height);
    glClearColor(col.x, col.y, col.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void KML::DrawFramebuffer(Framebuffer* framebuffer, Shader* shader, int w, int h) {
	assert(framebuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    UseShader(shader);
    glBindVertexArray(framebuffer->VAO);
    glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
    glBindTexture(GL_TEXTURE_2D, framebuffer->TEX);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
