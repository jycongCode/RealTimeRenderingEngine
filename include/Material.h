//
// Created by Lenovo on 2024/8/23.
//

#ifndef MATERIAL_H
#define MATERIAL_H
#include "Shader.h"

class Material {
public:
    std::string shaderName;
    virtual std::string GetType(){}
};

class MBlinnPhong:public Material{
public:
    MBlinnPhong(float ambient):ambient(ambient) {
        shaderName = "BlinnPhong";
    }
    float ambient;
    std::string GetType() override {
        return "BlinnPhong";
    }
};

class MBlinnPhong_Pure:public Material {
public:
    MBlinnPhong_Pure(float r,float g,float b,float ambient):ambient(ambient) {
        color = {r,g,b};
        shaderName = "BlinnPhong_Pure";
    }
    MBlinnPhong_Pure(glm::vec3 color,float ambient):color(color),ambient(ambient) {
        shaderName = "BlinnPhong_Pure";
    }
    std::string GetType() override {
        return "BlinnPhong";
    }
    glm::vec3 color;
    float ambient;
};

// scene stores the data vao vbo
// draw scene:
// draw skybox
// draw lightsource
// drawable draw() --> set model set material params, set

// deferred rendering
// 2 pass
// G-Buffer
// scene --> forward rendering
// quad render
// post processing ...
// 2 approaches
// 1. scene draw & store
// 2. scene store render draw
// 4. render.draw(Scene);

#endif //MATERIAL_H
