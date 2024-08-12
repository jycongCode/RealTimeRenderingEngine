//
// Created by Lenovo on 2024/8/12.
//

#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <glad/glad.h>
void Model::Destroy() {
}

void Model::loadModel(std::string path){
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,aiProcess_Triangulate|aiProcess_FlipUVs|aiProcess_CalcTangentSpace);
    if(!scene||scene->mFlags&AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cout << "ERROR::ASSIMP::"<<importer.GetErrorString() << std::endl;
    }
    directory = path.substr(0,path.find_last_of('/'));
    processNode(scene->mRootNode,scene);
}

void Model::processNode(aiNode *node, const aiScene* scene){
    for(unsigned int i = 0;i<node->mNumMeshes;++i){
        aiMesh*mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh,scene));
    }
    for(unsigned int i = 0;i<node->mNumChildren;++i){
        processNode(node->mChildren[i],scene);
    }
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for(unsigned int i = 0;i<mat->GetTextureCount(type);i++){
        aiString str;
        mat->GetTexture(type,i,&str);
        bool skip = false;
        for(unsigned int j = 0;j<textures_loaded.size();++j){
            if(std::strcmp(textures_loaded[j].path.C_Str(),str.C_Str()) == 0){
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }

        if(!skip) {
            Texture texture;
            directory;
            texture.LoadFromFile(str.C_Str(),directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        for(unsigned int i = 0;i<mesh->mNumVertices;++i){
            Vertex vertex;
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            if(mesh->mMaterialIndex >= 0){
                // process materials
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoord = vec;
            }
            vertices.push_back(vertex);
            // process vertex
        }

        for(unsigned int i = 0;i<mesh->mNumFaces;++i){
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0;j<face.mNumIndices;++j){
                indices.push_back(face.mIndices[j]);
            }
        }

        if(mesh->mMaterialIndex >=0) {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE,
                                                                    "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR,
                                                                     "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            std::vector<Texture> reflectMaps = loadMaterialTextures(material,aiTextureType_AMBIENT,
                                                                    "texture_reflect");
            textures.insert(textures.end(), reflectMaps.begin(), reflectMaps.end());
            std::vector<Texture> normalMaps = loadMaterialTextures(material,aiTextureType_HEIGHT,
                                                                   "texture_normal");
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        }
        return Mesh(vertices,indices,textures);
}
