//
// Created by Lenovo on 2024/8/13.
//

#ifndef _DISPLAY_COMPONENT
#define _DISPLAY_COMPONENT

#include "Component.h"
#include "WindowCallback.h"
#include <GLFW/glfw3.h>
class RTREngine;
class DisplayComponent:public Component
{
public:
    GLFWwindow* window;
public:
    void SetUp(RTREngine* engine) override;
    void Update(float deltaTime) override;
    void Destroy() override;
    void SetCallBacks(WindowCallback callbacks);
public:
    const unsigned int ScrWidth = 1920;
    const unsigned int ScrHeight = 1080;
};
#endif