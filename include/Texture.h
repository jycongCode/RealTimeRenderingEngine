//
// Created by Lenovo on 2024/8/12.
//

#pragma once
#include <string>
#include <assimp/scene.h>
#include <glm/glm.hpp>
class Texture {
public:
    void LoadFromFile(const char* filePath, bool sRGB);
    void CreateFromColor(glm::vec3 color,int size);
    unsigned int id;
    unsigned int type;
    aiString path;
};