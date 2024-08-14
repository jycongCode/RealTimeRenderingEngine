//
// Created by Lenovo on 2024/8/13.
//

#include <iostream>
#include <glad/glad.h>
#include "DisplayComponent.h"
#include "WindowCallback.h"
void DisplayComponent::setup() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(ScrWidth, ScrHeight, "RTREngine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glViewport(0,0,ScrWidth,ScrHeight);

}

void DisplayComponent::update(float deltaTime) {
    glfwSwapBuffers(window);
    glfwPollEvents();
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void DisplayComponent::destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void DisplayComponent::setCallBacks(WindowCallback callbacks) {
    glfwSetFramebufferSizeCallback(window, callbacks.framebuffer_size_callback);
    glfwSetCursorPosCallback(window,callbacks.mouse_pos_callback);
    glfwSetScrollCallback(window,callbacks.mouse_scroll_callback);
}

