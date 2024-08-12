//
// Created by Lenovo on 2024/8/12.
//

#pragma once
#include "Mesh.h"
#include <vector>
class Model{
private:
    std::vector<Texture> textures_loaded;
public:
    Model(char* path){
        loadModel(path);
    }

    void Destroy();
    std::vector<Mesh> meshes;
private:
    std::string directory;
    void loadModel(std::string path);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    void processNode(aiNode *node, const aiScene* scene);

    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,std::string typeName);
};
