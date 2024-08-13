//
// Created by Lenovo on 2024/8/12.
//

#pragma once
#include "Mesh.h"
#include <vector>
#include <unordered_map>

class Model{
private:
    std::unordered_map<std::string,Texture> textures_loaded;
public:
    Model(const char* path){
        loadModel(path);
    }

    void Destroy();
    std::vector<Mesh> meshes;
private:
    std::string directory;
    void loadModel(std::string path);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    void processNode(aiNode *node, const aiScene* scene);
    std::vector<Texture> processMaterial(aiMaterial* material,const aiScene* scene);
};
