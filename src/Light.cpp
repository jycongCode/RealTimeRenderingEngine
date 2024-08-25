//
// Created by Lenovo on 2024/8/16.
//

#include "Light.h"
#include "MathUtils.h"
glm::vec3 DirLight::GetDirVec3() {
    return MATHUTILS::EulerToVec3(direction);
}

std::string DirLight::GetTypeName() {
    return "directional";
}

std::string PointLight::GetTypeName() {
    return "point";
}