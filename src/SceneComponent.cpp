//
// Created by Lenovo on 2024/8/14.
//

#include "SceneComponent.h"
#include <string>
#include "Quad.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "DisplayComponent.h"
#include "RTREngine.h"
#include "MathUtils.h"
const char* INITIAL_PATH = "../scene/Marry.json";
glm::vec3 EulerToVector(glm::vec3 rotation) {
    glm::quat quat = MATHUTILS::EulerToQuat(rotation);
    glm::mat4 rotMat = glm::mat4_cast(quat);
    return glm::vec3(rotMat * glm::vec4(0.0,0.0,1.0,0.0));
}

void SceneComponent::SetUp(RTREngine*engine) {
    Component::SetUp(engine);
    Current = new Scene();
    Current->Load(INITIAL_PATH);
    Current->MainCamera.aspect = static_cast<float>(engine->displayComponent->ScrWidth) / static_cast<float>(engine->displayComponent->ScrHeight);
    CurrentID = Current->ID;
    CurrentStoragePath = Current->StoragePath;
}

void SceneComponent::Update(float deltaTime) {

}

void SceneComponent::Destroy() {
    Current->Destroy();
    delete(Current);
}
