//
// Created by Lenovo on 2024/8/14.
//

#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H


#include "SceneComponent.h"
class RenderComponent {
public:
    GLuint matricesUBO;
    GLuint LightUBO;
    std::unordered_map<std::string,Shader> shaderMap;
private:
    SceneComponent* sceneComponent = nullptr;
public:
    void addShader(const char* shaderName,const char* vsPath,const char* fsPath);
    void setup(SceneComponent& scene);
    void update(float deltaTime);
    void destroy();
};



#endif //RENDERCOMPONENT_H

/*
 * json --> model --> material[what kind of material]
 * shaderLibrary : all kinds of shaders shader1,shader2,shader3 .... id1, id2, id3
 * use material --> use corresponding shader --> set parameters;
 * load json --> keyID : model : mat
 */