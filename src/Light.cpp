//
// Created by Lenovo on 2024/8/16.
//

#include "Light.h"

std::string DirLight::GetTypeName() {
    return "directional";
}

std::string PointLight::GetTypeName() {
    return "point";
}