//
// Created by Lenovo on 2024/8/17.
//

#include <Shader.h>
#include "Quad.h"
float quadVertices[] = {
    // positions       //Normal           // tcd      //tangent
    -1.0f,  1.0f, 0.0f,0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,1.0f, 0.0f,
    1.0f,  1.0f,  0.0f,0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f,1.0f, 0.0f,
    1.0f,  1.0f,  0.0f,0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f,1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,1.0f, 0.0f,
    1.0f, -1.0f,  0.0f,0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f,1.0f, 0.0f
};

Quad::Quad() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    glBindVertexArray(0);
}

Quad::Quad(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale):Drawable(position,rotation,scale) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    glBindVertexArray(0);
}

void Quad::draw(Shader *shader) {
    shader->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES,0,6);
    glBindVertexArray(0);
}

void Quad::destroy() {
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);

}
