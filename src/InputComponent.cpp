//
// Created by Lenovo on 2024/8/14.
//
#include <glad/glad.h>
#include <iostream>
#include "InputComponent.h"

#include "Material.h"
#include "RTREngine.h"
#include "Scene.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
bool firstMouse = true;
float lastX = 0.0f;
float lastY = 0.0f;
Camera* camera = nullptr;
bool isEditMode = false;

// edit parameters
void mouse_pos_callback(GLFWwindow *window, double xpos, double ypos) {
    float xposIn = xpos;
    float yposIn = ypos;
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

void mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    if(isEditMode)
        camera->ProcessMouseScroll(yoffset);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0,0,width,height);
}


void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_RIGHT) {
        isEditMode = (action == GLFW_PRESS);
    }
}

void InputComponent::SetUp(RTREngine*engine) {
    Component::SetUp(engine);
    lastX = engine->displayComponent->ScrWidth / 2.0f;
    lastY = engine->displayComponent->ScrHeight / 2.0f;
    camera = &(engine->sceneComponent->Current->MainCamera);
    window = engine->displayComponent->window;

    WindowCallback callbacks;
    callbacks.mouse_pos_callback = mouse_pos_callback;
    callbacks.mouse_scroll_callback = mouse_scroll_callback;
    callbacks.framebuffer_size_callback = framebuffer_size_callback;
    callbacks.mouse_button_callback = mouse_button_callback;
    engine->displayComponent->SetCallBacks(callbacks);

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
void InputComponent::Update(float deltaTime) {
    glfwPollEvents();
    {
        if(window == nullptr) {
            std::cout << "InputComponent::update() called before setup()" << std::endl;
            return;
        }
        if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            Done = true;
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
        ImGui::Begin("Config Panel", nullptr, ImGuiWindowFlags_NoCollapse);

        if(ImGui::CollapsingHeader("Models")) {
            for(auto drawable : engine->sceneComponent->Current->DrawableList) {
                ImGui::SeparatorText(drawable->ID.c_str());
                ImGui::DragFloat3(("Position##"+drawable->ID).c_str(),&(drawable->Position[0]),0.2f,-FLT_MAX,FLT_MAX);
                ImGui::SliderFloat3(("Rotation##"+drawable->ID).c_str(),&(drawable->Rotation[0]),-360.0f,360.0f);
                ImGui::DragFloat3(("Scale##"+drawable->ID).c_str(),&(drawable->Scale[0]),0.1f,0.0f,10.0f);
                if(auto* ptr = dynamic_cast<MBlinnPhong*>(drawable->Mat)) {
                    ImGui::SliderFloat(("Ambient##"+drawable->ID).c_str(),&(ptr->ambient),0.0f,1.0f);
                }else if(auto* ptr = dynamic_cast<MBlinnPhong_Pure*>(drawable->Mat)) {
                    ImGui::SliderFloat(("Ambient##"+drawable->ID).c_str(),&(ptr->ambient),0.0f,1.0f);
                    ImGui::ColorEdit3(("Color##"+drawable->ID).c_str(),glm::value_ptr(ptr->color));
                }
            }
        }
        if(ImGui::CollapsingHeader("Lights")) {
            DirLight& sunLight = engine->sceneComponent->Current->Sunlight;
            ImGui::SliderFloat3("Direction##SunLight",glm::value_ptr(sunLight.direction),-360.0f,360.0f);
            ImGui::ColorEdit3("Color##SunLight",glm::value_ptr(sunLight.lightColor));
            ImGui::SliderFloat("Intensity##SunLight",&(sunLight.intensity),0.0f,10.0f);
            ImGui::SliderFloat("Right##SunLight",&(sunLight.right),0.0f,100.0f);
            ImGui::SliderFloat("Top##SunLight",&(sunLight.top),0.0f,100.0f);
            ImGui::SliderFloat("Near##SunLight",&(sunLight.nearp),0.0f,100.0f);
            ImGui::SliderFloat("Far##SunLight",&(sunLight.farp),0.0f,1000.0f);
            ImGui::SliderFloat("Distance##SunLight",&(sunLight.dis),0.0f,100.0f);
        }
        if(ImGui::CollapsingHeader("Camera")) {
            ImGui::SeparatorText(camera->ID.c_str());
            Camera& cam = engine->sceneComponent->Current->MainCamera;
            ImGui::Text("Postion:(%3f,%3f,%3f)",cam.Position.x,cam.Position.y,cam.Position.z);
            ImGui::Text("Pitch:%3f",cam.Pitch);
            ImGui::Text("Yaw:%3f",cam.Yaw);
            ImGui::Text("Zoom:%3f",cam.Zoom);
            ImGui::SliderFloat("Speed",&(cam.MovementSpeed),0.1f,10.0f);
        }
        ImGui::SeparatorText("Scene Config");
        if(ImGui::Button("Save File")) {
            engine->sceneComponent->Current->Save();
        }
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
    ImGui::Render();
}

void InputComponent::Destroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

// Display setup: window context opengl

// Input setup (display)