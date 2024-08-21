//
// Created by Lenovo on 2024/8/12.
//

#pragma once
#include "Mesh.h"
#include <vector>
#include <unordered_map>

class Model:public Drawable{
private:
    std::unordered_map<std::string,Texture> textures_loaded;
    std::string directory;
public:
    std::string filePath;
    std::vector<Mesh> meshes;
public:
    explicit Model(const char* path){
        loadModel(path);
        filePath = path;
    }
    Model(){};
    void draw(Shader shader) override;
    void loadModel(std::string path);
    void destroy() override;
private:
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    void processNode(aiNode *node, const aiScene* scene);
    std::vector<Texture> processMaterial(aiMaterial* material,const aiScene* scene);
};
