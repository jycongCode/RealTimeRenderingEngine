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
    std::string directory;
public:
    std::vector<Mesh> meshes;
    glm::mat4 model = glm::mat4(1.0f);
public:
    Model(const char* path){
        loadModel(path);
    }
    Model(){}
    void draw(Shader shader);
    void loadModel(std::string path);
    void destroy();
private:
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    void processNode(aiNode *node, const aiScene* scene);
    std::vector<Texture> processMaterial(aiMaterial* material,const aiScene* scene);
};
