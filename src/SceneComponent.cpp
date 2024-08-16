//
// Created by Lenovo on 2024/8/14.
//

#include "SceneComponent.h"

void SceneComponent::drawModel(const char *modelName, Shader shader) {
    if(modelMap.count(modelName) == 0) {
        std::cout << "SCENENCOMPONENT::ERROR[" << modelName << "]::Not in Scene" << std::endl;
        return;
    }
    Model model = modelMap[modelName];
    shader.use();
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(camera.Zoom,aspect,camera.nearPlane,camera.farPlane);
    glm::mat4 MVP = projection * view * model.model;
    shader.setMat4("MVP",MVP);
    shader.setMat4("M",model.model);
    shader.setVec3("cameraPos_wS",camera.Position);

    shader.setVec3("dirLight_wS",dirLight.Direction);
    shader.setVec3("lightColor",dirLight.lightColor);
    model.draw(shader);
}

void SceneComponent::addModel(const char *modelName, const char *modelPath) {
    Model model(modelPath);
    if(modelMap.count(modelName) > 0) {
        std::cout << "SCENENCOMPONENT::ERROR[" << modelName << "] already exists" << std::endl;
        return;
    }
    if(model.meshes.size()>0) {
        modelMap.insert({modelName,model});
    }
}

void SceneComponent::setup(DisplayComponent &display) {
    dirLight.lightColor = glm::vec3(0.5f,0.8f,0.4f);
    dirLight.Direction = glm::vec3(1.0f,-1.0f,-0.3f);
    aspect = static_cast<float>(display.ScrWidth) / static_cast<float>(display.ScrHeight);
}

void SceneComponent::update(float deltaTime) {

}

void SceneComponent::destroy() {

}

