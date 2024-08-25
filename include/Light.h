//
// Created by Lenovo on 2024/8/16.
//

#ifndef LIGHT_H
#define LIGHT_H
#include <glm/glm.hpp>
#include <string>

class LightBase {
public:
    std::string ID;
    LightBase(glm::vec3 color,float intensity):
        lightColor(color),intensity(intensity){}
    LightBase() = default;
    glm::vec3 lightColor;
    float intensity;
    virtual std::string GetTypeName(){};
};

class DirLight:public LightBase {
public:
    DirLight(std::string id,glm::vec3 dir = glm::vec3(1.0f,-1.0f,-0.3f),glm::vec3 color = glm::vec3(1.0f,1.0f,1.0f),float intensity = 1.0f):
        direction(dir),LightBase(color,intensity) {
        ID = id;
    }

    glm::vec3 GetDirVec3();
    DirLight() = default;
    std::string GetTypeName() override;
    glm::vec3 direction;
};

class PointLight:public LightBase {
public:
    PointLight(std::string id,glm::vec3 postion = glm::vec3(0.0f),float radius = 10.0f,glm::vec3 color = glm::vec3(1.0f,1.0f,1.0f),float intensity = 1.0f):
        position(position),radius(radius),LightBase(color,intensity) {
        ID = id;
    }
    std::string GetTypeName() override;
    glm::vec3 position;
    float radius;
};



#endif //LIGHT_H
