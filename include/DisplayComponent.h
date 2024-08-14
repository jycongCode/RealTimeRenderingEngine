//
// Created by Lenovo on 2024/8/13.
//

#ifndef _DISPLAY_COMPONENT
#define _DISPLAY_COMPONENT
#include <GLFW/glfw3.h>
#include <WindowCallback.h>
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
    void setup();
    void update(float deltaTime);
    void destroy();
    void setCallBacks(WindowCallback callbacks);
public:
    const unsigned int ScrWidth = 640;
    const unsigned int ScrHeight = 480;
};
#endif