//
// Created by Lenovo on 2024/8/14.
//


#ifndef _RTR_ENGINE
#define _RTR_ENGINE
#include "SceneComponent.h"
#include "InputComponent.h"
#include "DisplayComponent.h"
#include "RenderComponent.h"

class RTREngine {
private:
    float lastFrame = 0.0f;
    float deltaTime = 0.0f;
    void updateTime();
public:
    DisplayComponent displayComponent;
    SceneComponent sceneComponent;
    InputComponent inputComponent;
    RenderComponent renderComponent;
public:
    void setup();
    void run();
    void terminate();
};
#endif