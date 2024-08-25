//
// Created by Lenovo on 2024/8/12.
//

#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <glad/glad.h>
void Model::destroy() {
    for(auto mesh : meshes) {
        mesh.destroy();
    }
}

void Model::draw(Shader *shader) {
    for(auto & mesh : meshes) {
        mesh.draw(shader);
    }
}


void Model::loadModel(std::string filePath){
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath,aiProcess_Triangulate|aiProcess_FlipUVs|aiProcess_CalcTangentSpace);
    if(!scene||scene->mFlags&AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cout << "ERROR::ASSIMP::[" << filePath << "]" <<importer.GetErrorString() << std::endl;
    }
    directory = filePath.substr(0,filePath.find_last_of('/'));
    directory += "/";
    processNode(scene->mRootNode,scene);
    std::cout << "Model loaded: " << filePath << std::endl;
    std::cout << "Mesh Loaded: " << meshes.size() << std::endl;
    unsigned int num = 0;
    for(auto mesh : meshes)num += mesh.vertices.size();
    std::cout << "Vertex Loaded: " << num << std::endl;
    std::cout << "Texture Loaded: " << textures_loaded.size() << std::endl;
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
        if(mesh->HasTextureCoords(0)){
            // process materials
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoord = vec;
        }else{
            vertex.TexCoord = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    for(unsigned int i = 0;i<mesh->mNumFaces;++i){
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0;j<face.mNumIndices;++j){
            indices.push_back(face.mIndices[j]);
        }
    }

    textures = processMaterial(scene->mMaterials[mesh->mMaterialIndex],scene);
    return Mesh(vertices,indices,textures);
}

std::vector<Texture> Model::processMaterial(aiMaterial *material, const aiScene *scene) {
    std::vector<Texture> textures;

    //Finding current texture directory
    aiString texturePath;
    aiTextureType type;
    std::string fullTexturePath;

    //Checking all texture stacks for each texture type
    //Checkout assimp docs on texture types
    for(int tex = aiTextureType_NONE ; tex <= aiTextureType_UNKNOWN; tex++){
        type = static_cast<aiTextureType>(tex); //making the int value into the enum value
        fullTexturePath = directory;

        //If there are any textures of the given type in the material
        if( material->GetTextureCount(type) > 0 ){
            //We only care about the first texture assigned we don't expect multiple to be assigned
            material->GetTexture(type, 0, &texturePath);
            fullTexturePath = fullTexturePath.append(texturePath.C_Str());

            //If this texture has not been added to the atlas yet we load it
            if (textures_loaded.count(fullTexturePath) == 0){
                Texture texture;
                bool srgb = false;
                texture.LoadFromFile(fullTexturePath.c_str(), srgb);
                texture.type = type;
                texture.path = static_cast<aiString>(fullTexturePath);
                textures_loaded.insert({fullTexturePath, texture});
            }

            //We add it to the texture index array of loaded texture for a given mesh
            textures.push_back(textures_loaded[fullTexturePath]);
        }
        else{
            //For now we always assume that these textures will exist in the current
            //material. If they do not, we assign 0 to their value.
            //This will be fixed when the new material model is implemented.
            switch(type){
                case aiTextureType_LIGHTMAP:
                case aiTextureType_EMISSIVE:
                case aiTextureType_NORMALS:
                case aiTextureType_UNKNOWN:
                break;
            }
        }
    }
    return textures;
}