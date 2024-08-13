//
// Created by Lenovo on 2024/8/13.
//

#pragma once
#include <GLFW/glfw3.h>
class DisplayComponent {
private:
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    void updateTime(){
        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime-lastFrame;
        lastFrame = currentTime;
    }
public:
    GLFWwindow* window;
    const unsigned int ScrWidth = 640;
    const unsigned int ScrHeight = 480;
    void CreateWindow();
    void CreateContext();
    void update();
    void Destroy();
};
