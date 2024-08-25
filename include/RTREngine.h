//
// Created by Lenovo on 2024/8/14.
//


#ifndef _RTR_ENGINE
#define _RTR_ENGINE
class DisplayComponent;
class SceneComponent;
class InputComponent;
class RenderComponent;
class RTREngine {
private:
    float lastFrame = 0.0f;
    float deltaTime = 0.0f;
    void updateTime();
public:
    RTREngine();
    ~RTREngine();
    DisplayComponent* displayComponent = nullptr;
    SceneComponent* sceneComponent = nullptr;
    InputComponent* inputComponent = nullptr;
    RenderComponent* renderComponent = nullptr;
public:
    void SetUp();
    void Run();
    void Terminate();
};
#endif