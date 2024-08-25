//
// Created by Lenovo on 2024/8/14.
//

#include "RTREngine.h"
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "DisplayComponent.h"
#include "SceneComponent.h"
#include "RenderComponent.h"
#include "InputComponent.h"

RTREngine::RTREngine() {
    displayComponent = new DisplayComponent();
    renderComponent = new RenderComponent();
    inputComponent = new InputComponent();
    sceneComponent = new SceneComponent();
}

RTREngine::~RTREngine() {
    Terminate();
}
void RTREngine::updateTime() {
    auto currentTime = static_cast<float>(glfwGetTime());
    deltaTime = currentTime - lastFrame;
    lastFrame = currentTime;
}

void RTREngine::SetUp() {
    displayComponent->SetUp(this);
    sceneComponent->SetUp(this);
    renderComponent->SetUp(this);
    inputComponent->SetUp(this);
}

void RTREngine::Run() {
    while(!inputComponent->Done) {
        updateTime();
        if (glfwGetWindowAttrib(displayComponent->window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }
        inputComponent->Update(deltaTime);
        // glfwPollEvents();
        sceneComponent->Update(deltaTime);
        // none
        renderComponent->Update(deltaTime);
        // set shader vao
        // draw
        displayComponent->Update(deltaTime);
        // swap
        // clear
    }
}

void RTREngine::Terminate() {
    inputComponent->Destroy();
    sceneComponent->Destroy();
    renderComponent->Destroy();
    displayComponent->Destroy();
}
