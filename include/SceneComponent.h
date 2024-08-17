//
// Created by Lenovo on 2024/8/14.
//

#ifndef SCENE_COMPONENT
#define SCENE_COMPONENT
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
    std::unordered_map<std::string,Drawable*> modelMap;
    float aspect;
public:
    void draw(const char* modelName,Shader shader);
    void addModel(const char* modelName,const char* modelPath);
    void addDrawable(const char* name,Drawable* drawable);
    void setup(DisplayComponent& display);
    void update(float deltaTime) const;
    void destroy() const;
};
#endif
