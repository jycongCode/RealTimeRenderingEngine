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
     glGenBuffers(1,&matricesUBO);
     glBindBuffer(GL_UNIFORM_BUFFER,matricesUBO);
     glBufferData(GL_UNIFORM_BUFFER,128,nullptr,GL_STATIC_DRAW);
     glBindBufferBase(GL_UNIFORM_BUFFER,0,matricesUBO);
    sceneComponent = &scene;
    glEnable(GL_DEPTH_TEST);
}

void RenderComponent::update(float deltaTime) {
     glBindBuffer(GL_UNIFORM_BUFFER,matricesUBO);
     glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::mat4),glm::value_ptr(sceneComponent->GetProjectionMatrix()));
     glBufferSubData(GL_UNIFORM_BUFFER,sizeof(glm::mat4),sizeof(glm::mat4),glm::value_ptr(sceneComponent->GetViewMatrix()));
     glBindBuffer(GL_UNIFORM_BUFFER,0);

     for(auto [key,drawable] : sceneComponent->drawableMap) {
         sceneComponent->draw(key.c_str(),shaderMap["BlinnPhong"]);
     }
}

void RenderComponent::destroy() {
}
