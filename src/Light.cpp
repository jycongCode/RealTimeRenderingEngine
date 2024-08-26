//
// Created by Lenovo on 2024/8/16.
//

#include "Light.h"

#include "imgui_impl_opengl3_loader.h"
#include "MathUtils.h"
glm::vec3 DirLight::GetDirVec3() {
    return glm::normalize(direction);
}

std::string DirLight::GetTypeName() {
    return "directional";
}

glm::mat4 DirLight::GetLightMatrix(glm::vec3 center,float nearPlane,float farPlane) {
    glm::mat4 lightView = glm::lookAt(-dis * GetDirVec3(),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightProjection = glm::ortho(-right,right,-top,top,this->nearp,this->farp);
    return lightProjection * lightView;
}

std::string PointLight::GetTypeName() {
    return "point";
}
