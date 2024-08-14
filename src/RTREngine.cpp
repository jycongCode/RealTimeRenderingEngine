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
    sceneComponent.setup();
    inputComponent.setup(displayComponent,sceneComponent);
}

void RTREngine::run() {
    while(!inputComponent.done) {
        updateTime();
        sceneComponent.update(deltaTime);
        displayComponent.update(deltaTime);
        inputComponent.update(deltaTime);
    }
    sceneComponent.destroy();
    inputComponent.destroy();
    displayComponent.destroy();
}

void RTREngine::terminate() {
}
