//
// Created by Lenovo on 2024/8/12.
//

#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"
struct Vertex{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
    glm::vec3 Tangent;
};
class Mesh{
public :
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    Mesh(std::vector<Vertex> vertices,std::vector<unsigned int> indices,std::vector<Texture> textures){
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        setupMesh();
    }
    void Destroy();
private:
    unsigned int VAO,VBO,EBO;
    void setupMesh();
};