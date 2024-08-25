//
// Created by Lenovo on 2024/8/14.
//

#ifndef INPUT_COMPONENT
#define INPUT_COMPONENT
#include "SceneComponent.h"
#include "DisplayComponent.h"
class InputComponent:public Component{
private:
    GLFWwindow* window = nullptr;
    WindowCallback windowCallback;

public:
    // static void mouse_pos_callback(GLFWwindow* window,double xpos,double ypos);
    // static void mouse_scroll_callback(GLFWwindow* window,double xoffset,double yoffset);
    // static void framebuffer_size_callback(GLFWwindow* window,int width,int height);
    // static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
public:
    bool Done = false;
    void SetUp(RTREngine* engine) override;
    void Update(float deltaTime) override;
    void Destroy() override;
};
#endif