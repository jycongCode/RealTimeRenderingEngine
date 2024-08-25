//
// Created by Lenovo on 2024/8/12.
//

#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "Drawable.h"
#include "Shader.h"
#include "Texture.h"
struct Vertex{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
    glm::vec3 Tangent;
};

class Mesh:Drawable{
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
    Mesh() = default;
    void draw(Shader *shader) override;
    void destroy() override;
public:
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
    void setupMesh();
};