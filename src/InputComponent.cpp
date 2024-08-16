//
// Created by Lenovo on 2024/8/14.
//

#include "InputComponent.h"
#include <iostream>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
bool firstMouse = true;
float lastX = 0.0f;
float lastY = 0.0f;
Camera* camera = nullptr;
bool isEditMode = false;

// edit parameters
ImVec4 dirLightColor;

void InputComponent::param_init() {
    glm::vec3 pdirLightColor = sceneComponent->dirLight.lightColor;
    dirLightColor.x = pdirLightColor.x;
    dirLightColor.y = pdirLightColor.y;
    dirLightColor.z = pdirLightColor.z;
    dirLightColor.w = 1.0f;
}

void InputComponent::param_update() {
    glm::vec3 lightColor(dirLightColor.x,dirLightColor.y,dirLightColor.z);
    sceneComponent->dirLight.lightColor = lightColor;
}


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
    if(isEditMode)
        camera->ProcessMouseMovement(xoffset,yoffset,true);
}

void InputComponent::mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera->ProcessMouseScroll(yoffset);
}

void InputComponent::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0,0,width,height);
}


void InputComponent::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_RIGHT) {
        isEditMode = (action == GLFW_PRESS);
    }
}

void InputComponent::setup(DisplayComponent &display,SceneComponent& scene) {
    lastX = display.ScrWidth / 2.0f;
    lastY = display.ScrHeight / 2.0f;
    camera = &(scene.camera);
    sceneComponent = &scene;
    window = display.window;
    if(window == nullptr) {
        std::cout << "InputComponent::displayComponent not setup correctly" << std::endl;
    }
    WindowCallback callbacks;
    callbacks.mouse_pos_callback = mouse_pos_callback;
    callbacks.mouse_scroll_callback = mouse_scroll_callback;
    callbacks.framebuffer_size_callback = callbacks.framebuffer_size_callback;
    callbacks.mouse_button_callback = mouse_button_callback;
    this->windowCallback = callbacks;
    display.setCallBacks(callbacks);
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();
    param_init();
}

bool activateMouseRotate = false;
void InputComponent::update(float deltaTime) {
    glfwPollEvents();
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

    // imgui input
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
        float windowWidth = 60.0f; // 窗口宽度
        float windowHeight = 80.0f; // 窗口高度
        float xPos = ImGui::GetIO().DisplaySize.x - windowWidth - 50.0f; // 窗口的x位置，大致在屏幕右侧
        float yPos = 50.0f; // 窗口的y位置，这里是一个固定的值，但你可能需要根据你的UI布局进行调整

        // 注意：ImGui没有直接的参数来设置窗口的绝对位置，但你可以通过调整窗口的边界条件来近似实现
        // 一种方法是使用ImGui的窗口标志（如ImGuiWindowFlags_NoMove）来防止用户移动窗口，并通过内容对齐来影响窗口的最终位置
        // 然而，由于ImGui的布局机制，直接设置绝对位置并不总是可行的。因此，我们通常通过调整布局和窗口大小来近似达到所需的效果

        // 开始一个新的窗口，但注意我们没有直接设置位置
        ImGui::SetNextWindowPos(ImVec2(xPos, yPos), ImGuiCond_FirstUseEver); // 仅在窗口第一次创建时设置位置
        ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_FirstUseEver); // 仅在窗口第一次创建时设置大小
        ImGui::Begin("Fixed Window in Top Right Corner", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize); // 使用标志来防止窗口被折叠、调整大小或移动
        // 在这里绘制窗口内容
        ImGui::ColorEdit3("MyColor##1", (float*)&dirLightColor);
        // 结束窗口
        ImGui::End();
    }
    ImGui::Render();
    param_update();
}

void InputComponent::destroy() {
}


// Display setup: window context opengl

// Input setup (display)