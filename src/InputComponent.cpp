//
// Created by Lenovo on 2024/8/14.
//

#include "InputComponent.h"
#include <iostream>
bool firstMouse = true;
float lastX = 0.0f;
float lastY = 0.0f;
Camera* camera = nullptr;

void InputComponent::mouse_pos_callback(GLFWwindow *window, double xpos, double ypos) {
    float xposIn = static_cast<float>(xpos);
    float yposIn = static_cast<float>(ypos);
    if(firstMouse){
        lastX = xposIn,lastY = yposIn;
        firstMouse = false;
    }
    float xoffset = xposIn-lastX;
    float yoffset = lastY-yposIn;
    lastX = xposIn,lastY = yposIn;
    camera->ProcessMouseMovement(xoffset,yoffset, true);
}

void InputComponent::mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera->ProcessMouseScroll(yoffset);
}

void InputComponent::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0,0,width,height);
}

void InputComponent::setup(DisplayComponent &display,SceneComponent& scene) {
    lastX = display.ScrWidth / 2.0f;
    lastY = display.ScrHeight / 2.0f;
    camera = &(scene.camera);
    window = display.window;
    if(window == nullptr) {
        std::cout << "InputComponent::displayComponent not setup correctly" << std::endl;
    }
    WindowCallback callbacks;
    callbacks.mouse_pos_callback = mouse_pos_callback;
    callbacks.mouse_scroll_callback = mouse_scroll_callback;
    callbacks.framebuffer_size_callback = callbacks.framebuffer_size_callback;
    display.setCallBacks(callbacks);
}

void InputComponent::update(float deltaTime) {
    if(window == nullptr) {
        std::cout << "InputComponent::update() called before setup()" << std::endl;
        return;
    }
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS) {
       done = true;
    }
    if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS) {
        camera->ProcessKeyboard(Camera_Movement::FORWARD,deltaTime);
    }
    if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS) {
        camera->ProcessKeyboard(Camera_Movement::RIGHT,deltaTime);
    }
    if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS) {
        camera->ProcessKeyboard(Camera_Movement::LEFT,deltaTime);
    }
    if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS) {
        camera->ProcessKeyboard(Camera_Movement::BACKWARD,deltaTime);
    }
    if(glfwGetKey(window,GLFW_KEY_Q) == GLFW_PRESS) {
        camera->ProcessKeyboard(Camera_Movement::LIFT,deltaTime);
    }
    if(glfwGetKey(window,GLFW_KEY_E) == GLFW_PRESS) {
        camera->ProcessKeyboard(Camera_Movement::DROP,deltaTime);
    }
}

void InputComponent::destroy() {
}


// Display setup: window context opengl

// Input setup (display)