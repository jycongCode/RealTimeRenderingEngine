//
// Created by Lenovo on 2024/8/12.
//

#pragma once
#include <string>
#include <assimp/scene.h>
class Texture {
public:
    void LoadFromFile(const char* filePath, bool sRGB);
    unsigned int id;
    unsigned int type;
    aiString path;
};