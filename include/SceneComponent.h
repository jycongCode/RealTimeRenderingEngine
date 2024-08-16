//
// Created by Lenovo on 2024/8/14.
//

#ifndef _SCENE_COMPONENT
#define _SCENE_COMPONENT
#include "Camera.h"
#include "Shader.h"
#include "Model.h"
#include "Light.h"
#include "DisplayComponent.h"
#include <unordered_map>
class SceneComponent {
public:
    DirLight dirLight;
    Camera camera;
    std::unordered_map<std::string,Model> modelMap;
    float aspect;
public:
    void drawModel(const char* modelName,Shader shader);
    void addModel(const char* modelName,const char* modelPath);
    void setup(DisplayComponent& display);
    void update(float deltaTime);
    void destroy();
};
#endif
