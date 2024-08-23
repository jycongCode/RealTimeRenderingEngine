//
// Created by Lenovo on 2024/8/23.
//

#ifndef SCENE_H
#define SCENE_H
#include "Light.h"
#include "Camera.h"
#include "Drawable.h"
#include <vector>

class Scene {
public:
    std::string id;
    DirLight sunLight;
    Camera camera;
    std::vector<Drawable*> drawableList;
public:
    std::string filePath;
public:
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix() const;
    void Load(const char* path);
    void Save();
    void destroy();
};



#endif //SCENE_H
