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
}

// parameters

bool activateMouseRotate = false;
void InputComponent::update(float deltaTime) {
    glfwPollEvents();
    {
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
    // imgui input
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
        float windowWidth = 60.0f;
        float windowHeight = 80.0f;
        float xPos = ImGui::GetIO().DisplaySize.x - windowWidth - 50.0f;
        float yPos = 50.0f;

        ImGui::SetNextWindowPos(ImVec2(xPos, yPos), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_FirstUseEver);
        ImGui::Begin("Fixed Window in Top Right Corner", nullptr, ImGuiWindowFlags_NoCollapse);

        for(auto [key,drawable] : sceneComponent->modelMap) {
            ImGui::SeparatorText(key.c_str());
            ImGui::Text("address:%x",&(drawable->Position[0]));
            ImGui::DragFloat3(("Position##"+key).c_str(),&(drawable->Position[0]),0.2f,-FLT_MAX,FLT_MAX);
            ImGui::DragFloat3(("Rotation"+key).c_str(),&(drawable->Rotation[0]),0.1f,0.0f,360.0f);
            ImGui::DragFloat3(("Scale##"+key).c_str(),&(drawable->Scale[0]),0.1f,0.0f,10.0f);
        }

        ImGui::SeparatorText("Light");
        ImGui::ColorEdit3("MyColor##1", &(sceneComponent->dirLight.lightColor[0]));
        ImGui::SeparatorText("Camera");
        Camera& cam = sceneComponent->camera;
        ImGui::Text("Postion:(%3f,%3f,%3f)",cam.Position.x,cam.Position.y,cam.Position.z);
        ImGui::Text("Pitch:%3f",cam.Pitch);
        ImGui::Text("Yaw:%3f",cam.Yaw);
        ImGui::End();
    }
    ImGui::Render();
}

void InputComponent::destroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


// Display setup: window context opengl

// Input setup (display)