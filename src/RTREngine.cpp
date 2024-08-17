//
// Created by Lenovo on 2024/8/14.
//

#include "RTREngine.h"
#include <iostream>
#include <imgui_impl_glfw.h>
void RTREngine::updateTime() {
    float currentTime = glfwGetTime();
    deltaTime = currentTime - lastFrame;
    lastFrame = currentTime;
}

void RTREngine::setup() {
    displayComponent.setup();
    sceneComponent.setup(displayComponent);
    inputComponent.setup(displayComponent,sceneComponent);
    renderComponent.setup(sceneComponent);
}

void RTREngine::run() {
    bool flag = true;
    while(!inputComponent.done) {
        updateTime();
        if (glfwGetWindowAttrib(displayComponent.window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }
        inputComponent.update(deltaTime);
        sceneComponent.update(deltaTime);
        renderComponent.update(deltaTime);
        displayComponent.update(deltaTime);
        flag = false;

        /*
         * update time
         * scene update --> blank
         * render update --> render draw call
         * display update --> clear color
         * input poll events
         */
    }
    sceneComponent.destroy();
    inputComponent.destroy();
    displayComponent.destroy();
    renderComponent.destroy();
}

void RTREngine::terminate() {

}
