//
// Created by Lenovo on 2024/8/14.
//

#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "RTREngine.h"
#include "SceneComponent.h"
#include <unordered_map>
class RenderComponent:public Component {
public:
    GLuint MatricesUBO = 0;
    GLuint LightUBO = 0;
    GLuint CameraUBO = 0;
    GLuint ShadowMapFBO = 0;
    GLuint ShadowMap = 0;
private:
    std::unordered_map<std::string,Shader*> shaderLib;
public:
    RenderComponent() = default;
    void SetUp(RTREngine* engine) override;
    void Update(float deltaTime) override;
    void Destroy() override;
private:
    void LoadAndCompileShaders(const char* file);
    void SetUpUBOData();
    void GenerateShadowMap();
    void DrawDrawable();
    void DebugDraw(GLuint tex);
};



#endif //RENDERCOMPONENT_H

/*
 * json --> model --> material[what kind of material]
 * shaderLibrary : all kinds of shaders shader1,shader2,shader3 .... id1, id2, id3
 * use material --> use corresponding shader --> set parameters;
 * load json --> keyID : model : mat
 */