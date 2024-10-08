//
// Created by Lenovo on 2024/8/12.
//

#include "Mesh.h"
#include <glad/glad.h>

void Mesh::draw(Shader* shader) {
    int idx = 1;
    for(auto texture : textures) {
        if(texture.type == static_cast<unsigned int>(aiTextureType_DIFFUSE)) {
            shader->setInt("diffuse1",idx);
            glActiveTexture(GL_TEXTURE0+idx);
            glBindTexture(GL_TEXTURE_2D,texture.id);
            ++idx;
        }
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);
}

void Mesh::destroy() {
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
}

void Mesh::setupMesh() {
    //Generate Buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Bind Vertex Array Object and VBO in correct order
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //VBO stuff
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    //EBO stuff
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    //Vertex position pointer init
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    //Vertex normal pointer init
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    //Vertex texture coord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

    //Tangent position
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

    //Unbinding VAO
    glBindVertexArray(0);
}
