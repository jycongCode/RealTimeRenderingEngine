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
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class SceneComponent {
public:
    std::vector<LightBase*> lights;
    float envLight = 1.0f;
    Camera camera;
    std::unordered_map<std::string,Drawable*> modelMap;
    float aspect;
    std::string sceneFile;
    std::string sceneID;
public:
    void draw(const char* modelName,Shader shader);
    void addModel(const char* modelName,const char* modelPath,glm::vec3 position = glm::vec3(0.0f),glm::vec3 rotation = glm::vec3(0.0f),glm::vec3 scale = glm::vec3(1.0f));
    void addDrawable(const char* name,Drawable* drawable);
    void LoadScene(const char* filePath);
    void SaveScene(const char* filePath);
    void setup(DisplayComponent& display);
    void update(float deltaTime) const;
    void destroy() const;
};
#endif
