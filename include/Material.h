//
// Created by Lenovo on 2024/8/23.
//

#ifndef MATERIAL_H
#define MATERIAL_H
#include <string>
#include "Shader.h"
class Material {
public:
    std::string shaderName;
    virtual std::string GetType(){}
    virtual void Render(Shader* shader){}
};

class MBlinnPhong:public Material{
public:
    MBlinnPhong(float ambient,const char* shader):ambient(ambient) {
        shaderName = shader;
    }
    float ambient;
    std::string GetType() override {
        return "BlinnPhong";
    }

    void Render(Shader *shader) override {
        shader->use();
        shader->setFloat("ambient",ambient);
    }
};

class MBlinnPhong_Pure:public Material {
public:
    MBlinnPhong_Pure(float r,float g,float b,float ambient,const char* shader):ambient(ambient) {
        color = {r,g,b};
        shaderName = shader;
    }
    MBlinnPhong_Pure(glm::vec3 color,float ambient,const char* shader):color(color),ambient(ambient) {
        shaderName = shader;
    }
    std::string GetType() override {
        return "BlinnPhong_Pure";
    }

    void Render(Shader *shader) override {
        shader->use();
        shader->setFloat("ambient",ambient);
        shader->setVec3("color",color);
    }
    glm::vec3 color;
    float ambient;
};
#endif //MATERIAL_H
