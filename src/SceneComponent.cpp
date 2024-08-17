//
// Created by Lenovo on 2024/8/14.
//

#include "SceneComponent.h"

void SceneComponent::draw(const char *modelName, Shader shader) {
    if(modelMap.count(modelName) == 0) {
        std::cout << "SCENENCOMPONENT::ERROR[" << modelName << "]::Not in Scene" << std::endl;
        return;
    }
    Drawable* model = modelMap[modelName];
    shader.use();
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(camera.Zoom,aspect,camera.nearPlane,camera.farPlane);
    glm::mat4 MVP = projection * view * model->GetModelMatrix();
    shader.setMat4("MVP",MVP);
    shader.setMat4("M",model->GetModelMatrix());
    shader.setVec3("cameraPos_wS",camera.Position);

    shader.setVec3("dirLight_wS",dirLight.Direction);
    shader.setVec3("lightColor",dirLight.lightColor);
    model->draw(shader);
}

void SceneComponent::addModel(const char *modelName, const char *modelPath) {
    auto * model = new Model(modelPath);
    if(modelMap.count(modelName) > 0) {
        std::cout << "SCENENCOMPONENT::ERROR[" << modelName << "] already exists" << std::endl;
        return;
    }
    if(!model->meshes.empty()) {
        modelMap.insert({modelName,model});
    }
}

void SceneComponent::addDrawable(const char *name, Drawable *drawable) {
    if(modelMap.count(name) > 0) {
        std::cout << "SCENENCOMPONENT::ERROR[" << name << "] already exists" << std::endl;
        return;
    }
    modelMap.insert({name,drawable});
}

void SceneComponent::setup(DisplayComponent &display) {
    dirLight.lightColor = glm::vec3(0.5f,0.8f,0.4f);
    dirLight.Direction = glm::vec3(1.0f,-1.0f,-0.3f);
    aspect = static_cast<float>(display.ScrWidth) / static_cast<float>(display.ScrHeight);
}

void SceneComponent::update(float deltaTime) const {
}

void SceneComponent::destroy() const {
    for (auto [fst, snd] : modelMap) {
        if(snd != nullptr) {
            snd->destroy();
            delete(snd);
        }
    }
}

