//
// Created by Lenovo on 2024/8/14.
//

#include "RTREngine.h"
#include <iostream>
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
        sceneComponent.update(deltaTime);
        renderComponent.update(deltaTime);
        displayComponent.update(deltaTime);
        inputComponent.update(deltaTime);
        flag = false;
    }
    sceneComponent.destroy();
    inputComponent.destroy();
    displayComponent.destroy();
}

void RTREngine::terminate() {
}
