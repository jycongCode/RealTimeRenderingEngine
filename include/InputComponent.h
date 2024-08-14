//
// Created by Lenovo on 2024/8/14.
//

#ifndef _INPUT_COMPONENT
#define _INPUT_COMPONENT
#include "SceneComponent.h"
#include "DisplayComponent.h"
class InputComponent {
private:
    GLFWwindow* window = nullptr;
public:
    bool done = false;
    static void mouse_pos_callback(GLFWwindow* window,double xpos,double ypos);
    static void mouse_scroll_callback(GLFWwindow* window,double xoffset,double yoffset);
    static void framebuffer_size_callback(GLFWwindow* window,int width,int height);
    void setup(DisplayComponent& display,SceneComponent& scene);
    void update(float deltaTime);
    void destroy();
};
#endif