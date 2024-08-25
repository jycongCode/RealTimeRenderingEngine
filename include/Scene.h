//
// Created by Lenovo on 2024/8/23.
//

#ifndef SCENE_H
#define SCENE_H
#include "Drawable.h"
#include "Light.h"
#include "Camera.h"
#include <vector>

class Scene {
public:
    std::string ID;
    DirLight Sunlight;
    Camera MainCamera;
    std::vector<Drawable*> DrawableList;
public:
    std::string StoragePath;
public:
    glm::mat4 GetViewMatrix();
    [[nodiscard]] glm::mat4 GetProjectionMatrix() const;
    void Load(const char* path);
    void Save();
    void Save(const char* path);
    void Destroy();
};



#endif //SCENE_H
