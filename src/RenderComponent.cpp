//
// Created by Lenovo on 2024/8/14.
//
#include <glad/glad.h>
#include "RenderComponent.h"


 void RenderComponent::addShader(const char* shaderName,const char* vsPath,const char* fsPath){
    Shader shader(vsPath,fsPath);
    if(shaderMap.count(shaderName) > 0) {
        std::cout << "RENDERCOMPONENT::ERROR[" << shaderName << "] already exists" << std::endl;
        return;
    }
    if(shader.ID != 0) {
        shaderMap.insert({shaderName,shader});
    }
}

void RenderComponent::setup(SceneComponent &scene) {
    sceneComponent = &scene;
    glEnable(GL_DEPTH_TEST);
}

void RenderComponent::update(float deltaTime) {
     for(auto [key,m] : sceneComponent->modelMap) {
         sceneComponent->draw(key.c_str(),shaderMap["BlinnPhong"]);
     }
}

void RenderComponent::destroy() {
}
