//
// Created by Lenovo on 2024/8/16.
//

#ifndef LIGHT_H
#define LIGHT_H
#include <glm/glm.hpp>
#include <string>
#include "imgui_impl_opengl3_loader.h"

enum LightType {
    Directional,
    Point
};

struct LightBase {
    std::string ID;
    LightType type;
    LightBase(glm::vec3 color,float intensity):
        lightColor(color),intensity(intensity){}
    glm::vec3 lightColor;
    float intensity;
};

struct DirLight:LightBase {
    DirLight(std::string id,glm::vec3 dir = glm::vec3(1.0f,-1.0f,-0.3f),glm::vec3 color = glm::vec3(1.0f,1.0f,1.0f),float intensity = 1.0f):
        direction(dir),LightBase(color,intensity) {
        ID = id;
        type = LightType::Directional;
    }
    glm::vec3 direction;
};

struct PointLight:LightBase {
    PointLight(std::string id,glm::vec3 postion = glm::vec3(0.0f),float radius = 10.0f,glm::vec3 color = glm::vec3(1.0f,1.0f,1.0f),float intensity = 1.0f):
        position(position),radius(radius),LightBase(color,intensity) {
        ID = id;
        type = LightType::Point;
    }
    glm::vec3 position;
    float radius;
};



#endif //LIGHT_H
