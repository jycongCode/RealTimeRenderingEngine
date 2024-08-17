//
// Created by Lenovo on 2024/8/14.
//

#ifndef INPUT_COMPONENT
#define INPUT_COMPONENT
#include "SceneComponent.h"
#include "DisplayComponent.h"
class InputComponent {
private:
    GLFWwindow* window = nullptr;
    SceneComponent* sceneComponent = nullptr;
    WindowCallback windowCallback;
private:
    void param_init();

public:
    bool done = false;
    void param_update();
    static void mouse_pos_callback(GLFWwindow* window,double xpos,double ypos);
    static void mouse_scroll_callback(GLFWwindow* window,double xoffset,double yoffset);
    static void framebuffer_size_callback(GLFWwindow* window,int width,int height);
    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    void setup(DisplayComponent& display,SceneComponent& scene);
    void update(float deltaTime);
    void destroy();
};
#endif